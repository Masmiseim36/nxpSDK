/**
 *  \file appl_ga_legacy.c
 *
 *  Reference application for GA Core interfaces
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#ifdef BT_GAM

#include "appl_ga_legacy.h"

#ifdef APPL_GA_LEGACY_SUPPORT
#include "appl_ga_utils.h"
#include "appl_hci.h"
#include "appl_utils.h"

#include "appl_service.h"

#include "GA_pbp_api.h"

#ifdef APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA
#include "audio_pl.h"
#endif /* APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA */

#ifdef INCLUDE_ETHERMIND_LC3
#include "LC3_api.h"
#endif /* INCLUDE_ETHERMIND_LC3 */

#include "eBT_config.h"

/*
 * To avoid display of EA from the save device in
 * GA_BC_SOURCE_ANNOUNCEMENT_IND event.
 * If we have a device input from the user, Wait for EA from the device
 * and display all devices until the interested device is found.
 */
UINT8 found_bc_src = GA_FALSE;

GA_ENDPOINT bc_src_interested;

/**
 * Dump ISO Data to file.
 */
#define DUMP_ISO_DATA_TO_FILE

/* #define APPL_GA_CHAIN_CLIENT_PROCEDURES */
/* #define APPL_GA_CHAIN_SERVER_PROCEDURES */

#define GA_CONFIG_MAX_CHARS_PER_LINE 128
/* #define APPL_GA_USE_DEFAULT_VALUES */
/* --------------------------------------------- Global Definitions */
static const char ga_options[] = " \n\
================ GA MENU ================ \n\
   0.  Exit. \n\
   1.  Refresh this Menu. \n\
 \n\
   2.  GA Init. \n\
 \n\
   ****** Unicast Server ****** \n\
   3.  GA Register - PACS. \n\
   4.  GA Register ASEP - ASCS. \n\
 \n\
   5.  ASE Send Audio Context as Available. \n\
   6.  ASE Send Auto Codec Configured. \n\
   7.  ASE Send Auto Rx Start Ready for Local ASE Sink. \n\
   8.  ASE Send Auto Disable. \n\
   9.  ASE Send Auto Update Metadata. \n\
   10. ASE Send Auto Release. \n\
   11. ASE Release Complete. \n\
 \n\
   15. Save Audio Dump to Files. \n\
   16. Convert PCM to Wav. \n\
   17. Convert Wav to PCM. \n\
 \n\
   ****** Unicast Client ****** \n\
   20. Setup PACS - Audio Role Discovery. \n\
   21. Release/Close PACS. \n\
   22. Get PACS Capabilities - Audio Capability, Location, Supp/Avail Contexts \n\
 \n\
   25. ASE Discover - ASE_ID Discovery. \n\
   26. ASE Codec Configuration. \n\
   27. ASE Setup CIG Parameters. \n\
   28. ASE QoS Configuration. \n\
   29. ASE Enable. \n\
   30. ASE Setup CIS. \n\
   31. ASE Receiver Start Ready. \n\
   32. ASE Update. \n\
   33. ASE Disable. \n\
   34. ASE Receiver Stop Ready for Local ASE Sink. \n\
   35. ASE Release. \n\
\n\
   36. ASE Send Data. \n\
   37. ASE Release ISOC Data Channel. \n\
\n\
   38. ASE Accept CIS. \n\
\n\
   40. VCP Controller Operations. \n\
   41. VCP Renderer Operations. \n\
\n\
   42. MICP Controller Operations. \n\
   43. MICP Device Operations. \n\
\n\
   44. CSIP Set Coordinator Operations. \n\
   45. CSIP Set Member Operations. \n\
\n\
   50. LE Adv. Enable. \n\
   51. LE Connection \n\
   52. LE Extended Connection \n\
\n\
   55. LE Authenticate and Encrypt \n\
\n\
   60. Set peer endpoint \n\
\n\
   ****** Broadcast Source ****** \n\
   80. Allocate Session \n\
   81. Free Session \n\
   82. Configure Session \n\
   83. Register Source EndPoint \n\
   84. Setup Announcement \n\
   85. End Announcement \n\
   86. Start Broadcast \n\
   87. Suspend Broadcast \n\
\n\
   ****** Broadcast Sink ****** \n\
   90. Scan Announcement \n\
   91. Scan End \n\
   92. Associate Source \n\
   93. Dissociate Source \n\
   94. Enable Stream \n\
   95. Disable Stream \n\
\n\
  100. GA Shutdown \n\
  101. Update Ntf Configuration for BAP \n\
\n\
  105. Get GA context information \n\
  106. Set GA context information \n\
\n\
   ****** Automation ****** \n\
  110. Start Server - Sink \n\
  111. Start Client - Source \n\
\n\
Your Option ?: ";

/* Server */
void appl_ga_register(void);
void appl_ga_automate_register_pacs(void);
void appl_ga_register_ascs(void);
void appl_ga_register_dynamic_db(void);
void appl_ga_automate_unicast_server_operations(void);

/* Client */
void appl_ga_unicast_set_remote_dev(void);
void appl_ga_unicast_auth_and_encrypt(void);
void appl_ga_unicast_client_timer_expiry_handler
     (
         void   * data,
         UINT16   datalen
     );

void appl_ga_unicast_client_setup(void);
void appl_ga_unicast_client_get_pacs_capabilities(void);
void appl_ga_unicast_client_discover_ascs(void);
void appl_ga_unicast_client_configure_codec(void);
void appl_ga_unicast_client_setup_cig(void);
void appl_ga_unicast_client_configure_qos(void);
void appl_ga_unicast_client_enable(void);
void appl_ga_unicast_client_create_cis(void);
void appl_ga_automate_unicast_client_operations(void);

static BT_timer_handle appl_ga_unicast_client_timer_handle;

void appl_ga_read_config_file(void);

void appl_ga_unicast_server_adv_enable(void);

/* Maximum Audio Capabilities supported locally,
 * Should Map to GA_MAX_SOURCE_PAC_RECORDS and GA_MAX_SINK_PAC_RECORDS
 *
 * Also, Maximum Audio Capabilities exposed by remote.
 */
#define APPL_GA_MAX_AUDIO_CAPABILITIES   4

/* Maximum ASE supported locally,
 * Should Map to BAP_MAX_ASE
 */
#define APPL_GA_MAX_ASE 4

/* Maximum simultaneous connections */
#define APPL_GA_MAX_DEVICES 2

 /* ASE States maintained in the Application, Does conform to spec */
typedef UINT8 APPL_GA_ASE_STATE;

#define APPL_GA_STATE_REGISTERED     0x00
#define APPL_GA_STATE_CODEC_CONF     0x01
#define APPL_GA_STATE_QOS_CONF       0x02
#define APPL_GA_STATE_ENABLING       0x03
#define APPL_GA_STATE_STREAMING      0x04
#define APPL_GA_STATE_DISABLING      0x05
/* This state indicates that the CIS and the ISO Data path exists */
#define APPL_GA_STATE_SUSPENDED      0x06
#define APPL_GA_STATE_RELEASING      0x07

/* ISO Data Direction For Setup */
#define APPL_GA_ISO_UNKNOWN          0xFF
#define APPL_GA_ISO_TX                 0
#define APPL_GA_ISO_RX                 1

/* Number of channes */
#define APPL_LC3_CHANNELS_MAX          1

/* ISO Data Direction for Removal */
#define APPL_GA_ISO_INPUT            0x01 /* Bit 0 */  /* Host to Controller */
#define APPL_GA_ISO_OUTPUT           0x02 /* Bit 1 */  /* Controller to Host */

/* ASE CP Response */
#define GA_APPL_ASECP_RSP_SUCCESS                    0x00
#define GA_APPL_ASECP_RSP_UNSUPP_OPCODE              0x01
#define GA_APPL_ASECP_RSP_INVALID_LEN                0x02
#define GA_APPL_ASECP_RSP_INVALID_ASE_ID             0x03
#define GA_APPL_ASECP_RSP_INVALID_ASE_SM_TRANSITION  0x04
#define GA_APPL_ASECP_RSP_INVALID_ASE_DIRECTION      0x05
#define GA_APPL_ASECP_RSP_UNSUPP_AUDIO_CAP           0x06
#define GA_APPL_ASECP_RSP_UNSUPP_CONFIG_PARAM_VALUE  0x07
#define GA_APPL_ASECP_RSP_REJ_CONFIG_PARAM_VALUE     0x08
#define GA_APPL_ASECP_RSP_INVALID_CONFIG_PARAM_VALUE 0x09
#define GA_APPL_ASECP_RSP_UNSUPP_METADATA            0x0A
#define GA_APPL_ASECP_RSP_REJ_METADATA               0x0B
#define GA_APPL_ASECP_RSP_INVALID_METADATA           0x0C
#define GA_APPL_ASECP_RSP_INSUFF_RESOURCES           0x0D
#define GA_APPL_ASECP_RSP_UNSPECIFIED_ERR            0x0E

#define APPL_GA_GATT_SERVER                          0x00
#define APPL_GA_GATT_CLIENT                          0x01
#define APPL_GA_GATT_NONE                            0x02

typedef struct _APPL_GA_ASE_
{
    /* Only applicable to Local Server Role */
    GA_SEP_CONF codec_conf;
    GA_QOS_CAP qos_cap;
    GA_QOS_CONF qos_conf;
    /* Only applicable to Local Client Role, since codec_conf is not present
     * This refers to ASE ID role, remote ASE Role.
     */
    UINT8 ase_role;
    UINT8 ase_id;
    APPL_GA_ASE_STATE curr_state;
    UINT16 cis_conn_handle;
    UINT16 acl_conn_handle;
    UINT16 bytes_to_lc3_encode;
    INT32 lc3_encoded_bytes;
    /* CIS Setup - sca */
    UCHAR sca;
    /* CIS Setup - packing */
    UCHAR packing;

}APPL_GA_ASE;

typedef struct _APPL_GA_REM_DEV_INFO_
{
    /* Local Client DB */
    GA_ENDPOINT ga_device;
    GA_CONTEXT ga_context;

    /* Remote PACS Records of device */
    GA_AUDIO_CAP ga_rem_audio_cap[APPL_GA_MAX_AUDIO_CAPABILITIES];
    GA_PACS_AUDIO_RECORD ga_rem_pacs;
    UINT8 ga_rem_audio_cap_count;

    APPL_GA_ASE ga_rem_ase[APPL_GA_MAX_ASE];
    UINT8 ga_rem_ase_count;
    UINT16 acl_conn_handle;

}APPL_GA_REM_DEV_INFO;

/* PACS Local Audio Capabilities */
DECL_STATIC GA_AUDIO_CAP ga_local_audio_cap[APPL_GA_MAX_AUDIO_CAPABILITIES];
DECL_STATIC UINT8 ga_local_audio_cap_count;
DECL_STATIC GA_PACS_AUDIO_RECORD ga_local_pacs;

/* ASCS Local ASE supported */
DECL_STATIC APPL_GA_ASE ga_local_audio_ase[APPL_GA_MAX_ASE];
DECL_STATIC UINT8 ga_local_ase_count;

/* Remote Device Info - Local Client */
DECL_STATIC APPL_GA_REM_DEV_INFO ga_rem_dev_info[APPL_GA_MAX_DEVICES];
DECL_STATIC UINT8 curr_rem_index;

/* Currently only GATT Client or GATT Server is supported at a given point */
DECL_STATIC UINT8 curr_GATT_role;

/* Current value set for both GATT Client or GATT Server */
UINT16 bytes_to_lc3_encode;
UINT8 ga_local_audio_src_streaming;
UINT16 active_iso_handle;

DECL_STATIC GA_ENDPOINT appl_ga_device;
DECL_STATIC GA_CONTEXT appl_ga_context;

#ifdef BAP_BROADCAST
DECL_STATIC UINT8 appl_bcast_ssn;
DECL_STATIC UINT8 appl_bcast_phy;
DECL_STATIC UINT8 appl_bcast_packing;
DECL_STATIC UINT8 encryption_state;
DECL_STATIC UCHAR appl_bcast_code[GA_BROADCAST_CODE_SIZE] = \
                  {0x00U, 0x01U, 0x02U, 0x03U, 0x04U, 0x05U, 0x06U, 0x07U, 0x08U, 0x09U, 0x0AU, 0x0BU, 0x0CU, 0x0DU, 0x0EU, 0x0FU};

DECL_STATIC GA_CODEC_INFO appl_bcast_codecinfo;
DECL_STATIC GA_METADATA appl_bcast_metainfo;
DECL_STATIC UINT8 appl_bcast_nstream = 1U;
DECL_STATIC GA_CODEC_IE appl_bcast_stream_codec_ie;
DECL_STATIC UINT8 appl_bcast_sep;

DECL_STATIC UINT32 appl_bcast_id = 0xDEADBEU;
DECL_STATIC UINT32 appl_bcast_presentation_delay;
DECL_STATIC UINT32 appl_bcast_sdu_interval;
DECL_STATIC UINT16 appl_bcast_max_sdu;
DECL_STATIC UINT16 appl_bcast_max_latency;
DECL_STATIC UINT8 appl_bcast_rtn;
DECL_STATIC UINT8 appl_bcast_framing;

DECL_STATIC UINT8 appl_bcast_source_sid;
DECL_STATIC UINT16 appl_bcast_synchandle = 0xFFFF;
DECL_STATIC UINT16 appl_bcast_config_synchandle = 0xFFFF;
DECL_STATIC UINT16 appl_bcast_enable_synchandle = 0xFFFF;
DECL_STATIC UINT16 appl_bcast_stream_synchandle = 0xFFFF;
DECL_STATIC UINT8 appl_bcast_bis_count = 1U;
DECL_STATIC UINT8 appl_bcast_bis[1U] = {0x01U};
#endif /* BAP_BROADCAST */

/* #define CONFIG_16_2 */

/* #define CONFIG_24_2 */
#define CONFIG_48_2
/*
 * Codec Specific Capabilities:
 * Length: 1B
 * Refer Codec_Specific_Capabilities (Defined in PACS [5])
 * 16   LC3 16  10  40 (32 kbps)    M   M
 * Sampling Freq = 16
 * Codec_ID = LC3
 * Supp Frame duration = 10ms
 * Supp Bytes per codec frame = 40Bytes.
 *
 * bitrate = samp freq * bit depth(sample size) * No of channels
 * Bitrate = 32kbps.
 *
 * Each Sample size includes n num of bits:
 * bits per sample = samples per second * bits per sample
 *                 =  2 * 8 = 16bits per sample.
 *
 * Value:
 * Metadata Length:
 * Metadata Value:
 */
#ifdef CONFIG_16_2
#define APPL_GA_CODEC_SUPP_SAMPL_FREQ            16000
#define APPL_GA_CODEC_MAX_SDU                    40
#define APPL_GA_CODEC_BITRATE_BPS                32000

#define APPL_GA_CODEC_SUPP_FRAME_DURATION_MS     100
#define APPL_GA_SAMPLE_SIZE_IN_BITS              16

#endif /* CONFIG_16_2 */

#ifdef CONFIG_24_2
#define APPL_GA_CODEC_SUPP_SAMPL_FREQ            24000
#define APPL_GA_CODEC_MAX_SDU                    60
#define APPL_GA_CODEC_BITRATE_BPS                48000

#define APPL_GA_CODEC_SUPP_FRAME_DURATION_MS     100
#define APPL_GA_SAMPLE_SIZE_IN_BITS              16

#endif /* CONFIG_24_2 */

#ifdef CONFIG_48_2
#define APPL_GA_CODEC_SUPP_SAMPL_FREQ            48000
#define APPL_GA_CODEC_MAX_SDU                    100
#define APPL_GA_CODEC_BITRATE_BPS                80000 /* 96000 124000 80000 */

#define APPL_GA_CODEC_SUPP_FRAME_DURATION_MS     100
#define APPL_GA_SAMPLE_SIZE_IN_BITS              16

#endif /* CONFIG_48_2 */

/*
 * Refer QoS Configuration settings for low latency audio data
 * 16_2_1   16_2    10000   unframed    40 (32 kbps)    2   10  40000   M   M
 * SDU Interval(micro sec) = 10000
 * Framing = Unframed = 0
 * Max SDU Size = 40Bytes.
 * ReTxn = 2
 * Max Transport Latency = 10 = 0x0A
 * Presentation Delay(micro sec) = 40000 = 0x9c40
 * Bitrate = 32kbps.
 */
#ifdef CONFIG_16_2
#define APPL_GA_FRAMING_UNFRAMED            0
#define APPL_GA_PREF_PHY                    1
#define APPL_GA_PREF_MAX_TRANS_LATENCY      0x0A
#define APPL_GA_PREF_PRESENTATION_DELAY_MIN 0
#define APPL_GA_PREF_PRESENTATION_DELAY_MAX 0x9C40
#define APPL_GA_PREF_RETXN                  2
#define APPL_GA_SUPP_PRESENTATION_DELAY_MIN 0
#define APPL_GA_SUPP_PRESENTATION_DELAY_MAX 0x9C40

/* TODO: MicroSec */
#define APPL_GA_SDU_INTERVAL                10000

#endif /* CONFIG_16_2 */

#ifdef CONFIG_24_2
#define APPL_GA_FRAMING_UNFRAMED            0
#define APPL_GA_PREF_PHY                    1
#define APPL_GA_PREF_MAX_TRANS_LATENCY      0x0A
#define APPL_GA_PREF_PRESENTATION_DELAY_MIN 0
#define APPL_GA_PREF_PRESENTATION_DELAY_MAX 0x9C40
#define APPL_GA_PREF_RETXN                  2
#define APPL_GA_SUPP_PRESENTATION_DELAY_MIN 0
#define APPL_GA_SUPP_PRESENTATION_DELAY_MAX 0x9C40

/* TODO: MicroSec */
#define APPL_GA_SDU_INTERVAL                10000

#endif /* CONFIG_24_2 */

#ifdef CONFIG_48_2
#define APPL_GA_FRAMING_UNFRAMED            0
#define APPL_GA_PREF_PHY                    1
#define APPL_GA_PREF_MAX_TRANS_LATENCY      0x5F
#define APPL_GA_PREF_PRESENTATION_DELAY_MIN 0
#define APPL_GA_PREF_PRESENTATION_DELAY_MAX 0x9C40
#define APPL_GA_PREF_RETXN                  13
#define APPL_GA_SUPP_PRESENTATION_DELAY_MIN 0
#define APPL_GA_SUPP_PRESENTATION_DELAY_MAX 0x9C40

/* TODO: MicroSec */
#define APPL_GA_SDU_INTERVAL                10000

#endif /* CONFIG_48_2 */

#define APPL_GA_ASE_SCA                     0 /* 251 ppm to 500 ppm */
#define APPL_GA_ASE_PACKING_SEQUENTIAL      0

#define APPL_GA_ASE_CHANNEL_COUNT           1

/* TODO: How this is decided??? */
#define APPL_GA_ISO_CONTROLLER_DELAY        0

#define APPL_GA_ISO_HEADER                  12 /* Without TimeStamp */

/* Maximum ISO Dummy Data to be sent via ISOC Channel */
#define APPL_GA_ISO_DUMMY_MAX_DATALEN       40 /* 10 */
DECL_STATIC UCHAR appl_ga_iso_dummy_data[APPL_GA_ISO_DUMMY_MAX_DATALEN];

#define APPL_GA_MAX_CIS_COUNT 3

/* Encoder and Decoder States */
DECL_STATIC UINT8 appl_ga_lc3_enc_ready = GA_FALSE;
DECL_STATIC UINT8 appl_ga_lc3_dec_ready = GA_FALSE;

/* Queue: Synchronization for thread */
BT_DEFINE_MUTEX(appl_ga_src_th_mutex)
BT_DEFINE_COND(appl_ga_src_th_cond)

/**
 * Maximum Bytes to Encode will always be not more than MAX SDU.
 * Depends on APPL_GA_CODEC_BITRATE_BPS, APPL_GA_CODEC_SUPP_SAMPL_FREQ,
 * APPL_GA_CODEC_SUPP_FRAME_DURATION_MS
 * Setting a random value now.
 */
#define APPL_GA_MAX_BYTES_TO_LC3_ENCODE       (960U * APPL_GA_ASE_CHANNEL_COUNT)

 /**
 * Src PCM Queue Size, should be a power of 2 value.
 * This value shall be at least double of the data length being enqueued
 * every time.
 * Depends on APPL_GA_CODEC_BITRATE_BPS, APPL_GA_CODEC_SUPP_SAMPL_FREQ,
 * APPL_GA_CODEC_SUPP_FRAME_DURATION_MS
 * Setting a random value now.
 */
#define APPL_GA_MAX_BUFFER_SIZE               (APPL_GA_MAX_BYTES_TO_LC3_ENCODE << 2U)


/* Maximum Data Buffer Length to be fed to LC3 Encoder at once */
#define APPL_GA_BLOCK_MAX

#define APPL_GA_SRC_WR_TH_INIT                 0x01U
#define APPL_GA_SRC_WR_TH_STOP                 0x02U
#define APPL_GA_SRC_WR_TH_PLAYING              0x03U

 /* Write thread state */
static UINT8 appl_ga_src_wr_th_state;

/* Queue - Buffers and Rd/Wr indices */
static INT32 appl_ga_src_buffer_rd_ptr;
static INT32 appl_ga_src_buffer_wr_ptr;
static INT32 appl_ga_src_buffer_size;
static UCHAR appl_ga_src_buffer[APPL_GA_MAX_BUFFER_SIZE];

/* PCM Buffer for Encoding */
static UCHAR appl_ga_src_pcm_to_send[APPL_GA_MAX_BYTES_TO_LC3_ENCODE];

/* Indicate if Silence Buffer is needed post ISO Data Path Setup */
UCHAR isISODataPathReset = GA_FALSE;

/* Current Status of an active ASE Source */
typedef struct _APPL_GA_CURR_SRC_ASE_
{
    UINT16                    acl_conn_handle;
    UINT16                    cis_conn_handle;
    APPL_GA_ASE_STATE  state;

}APPL_GA_CURR_SRC_ASE;

void appl_ga_prep_for_streaming(UINT8 local_ase_db_index);

/*
 * Callback that is triggered from Audio Pl callback, Call the Queue
 * Module to stagger data flow
 */
static void appl_ga_audio_pl_src_callback
            (
                /* IN */ const UCHAR * p_data,
                /* IN */ UINT16        p_datalen
            );

static void appl_ga_audio_pl_snk_callback
            (
                /* IN */ const UCHAR * p_data,
                /* IN */ UINT16        p_datalen
            );

/**
 * Queue implementation to match rate of data to be encoded and fed to ISO.
 * Audio PL layer generates audio at a rate higher than that required to be
 * fed to the ISO layer(ISO demands data based on SDU Interval.
 */
GA_RESULT appl_ga_src_enqueue
          (
              /* IN */  const UCHAR * data,
              /* IN */  UINT16        datalen
          );

/**
 * Task that is waiting for PCM data from Audio PL Layer in the
 * queue. This task picks the data as per number of bytes to be fed to
 * LC3 Encoder. The number of bytes is based on the codec configuration.
 */
DECL_STATIC BT_THREAD_RETURN_TYPE appl_ga_src_write_task
                                  (
                                      BT_THREAD_ARGS args
                                  );

GA_RESULT appl_ga_fetch_local_ase_db_index_from_cig_cis_id
          (
              UINT16 acl_conn_handle,
              UINT8 cig_id,
              UINT8 cis_id,
              UINT8* ase_db_id,
              UINT8* ase_count
          );
GA_RESULT appl_ga_fetch_local_ase_db_index_from_acl_conn_handle
          (
              UINT16 acl_conn_handle,
              UINT8* ase_db_id,
              UINT8* ase_count
          );
GA_RESULT appl_ga_fetch_local_ase_db_index_from_cis_conn_handle
          (
              UINT16 cis_conn_handle,
              UINT8* ase_db_id,
              UINT8* ase_count
          );
GA_RESULT appl_ga_fetch_rem_ase_db_index_from_cig_id
          (
              UINT8 cig_id,
              UINT8* ase_db_id
          );
GA_RESULT appl_ga_fetch_rem_ase_db_index_from_acl_conn_handle
          (
              UINT16 acl_conn_handle,
              UINT8* ase_db_id
          );
GA_RESULT appl_ga_fetch_rem_ase_db_index_from_cis_conn_handle
          (
              UINT16 cis_conn_handle,
              UINT8* ase_db_id,
              UINT8* ase_count
          );
GA_RESULT appl_ga_fetch_local_ase_db_index_from_ase_index
          (
              UINT8 ase_id,
              UINT8* ase_db_id
          );
GA_RESULT appl_ga_fetch_rem_ase_db_index_from_ase_id
          (
              UINT8 ase_id,
              UINT8* ase_db_id
          );
GA_RESULT appl_ga_fetch_rem_free_ase_db_index(UINT8* ase_db_id);
GA_RESULT appl_ga_fetch_rem_free_index
          (
              UINT8 * rem_dev_index
          );
GA_RESULT appl_ga_fetch_rem_index_from_ctx
          (
              GA_CONTEXT *ctx,
              UINT8 * rem_dev_index
          );

void appl_ga_create_codec_spec_cap_ltv
     (
         GA_LC3_CODEC_SPECIFIC_CAP * codec_specific_cap,
         UCHAR * ltvarray,
         UINT8 * ltvarray_len
     );

void appl_ga_create_codec_spec_conf_ltv
     (
         GA_LC3_CODEC_SPECIFIC_CONF * codec_specific_conf,
         UCHAR * ltvarray,
         UINT8 * ltvarray_len
     );

/** BAP Client **/
GA_RESULT appl_bap_unicast_client_callback
          (
              GA_ENDPOINT   * ga_device,
              UINT8    ga_event,
              UINT16   ga_status,
              void   * ga_data,
              UINT16   ga_datalen
          );

/** BAP Server **/
GA_RESULT appl_bap_unicast_server_callback
          (
              GA_ENDPOINT * device,
              UINT8         ga_event,
              UINT16        ga_status,
              void        * ga_data,
              UINT16        ga_datalen
          );

#ifdef BAP_BROADCAST
void appl_ga_prep_bcast_for_streaming(UINT8 direction, UINT16 bis_conn_hndl);

GA_RESULT appl_bap_broadcast_source_callback
          (
              GA_ENDPOINT  * device,
              UINT8    ga_event,
              UINT16   ga_status,
              void   * ga_data,
              UINT16   ga_datalen
          );

/** BAP Server **/
GA_RESULT appl_bap_broadcast_sink_callback
          (
              GA_ENDPOINT  * device,
              UINT8    ga_event,
              UINT16   ga_status,
              void   * ga_data,
              UINT16   ga_datalen
          );

#endif /* BAP_BROADCAST */

GA_RESULT appl_ga_hci_iso_data_ind_cb
          (
              UCHAR* header,
              UCHAR* data,
              UINT16 datalen
          );

#ifdef INCLUDE_ETHERMIND_LC3

#define APPL_GA_LC3_SILENCE_BUFFER_COUNT 4

/**
 * This function uses the PCM Data fed from the task, this calls the
 * LC3 Encoder API to derive the LC3 encoded data. This will trigger a
 * call to the ISO Data Channel to send the LC3 encoded data over ISO
 * channel.
 * The number of Bytes to be fed to LC3 codec is:
 * SF(inKHz) * (Interval *10)/10 * nofchannels *2)
 * The number of Bytes encoded is:
 * bitrate * (INT32)n_f_len / (8 * sample_rate)
 * Refer LC3_encoder_convert_bitrate_to_bytes()
 */
GA_RESULT lc3_encode_n_send
          (
              UCHAR  * pcm_data,
              UINT16   pcm_datalen
          );

GA_RESULT appl_ga_lc3_decode_n_play
          (
              UCHAR  * data,
              UINT16   datalen
          );

/* LC3 Encoder and Decoder Context */
#define APPL_GA_LC3_DEC_PLC_TYPE_DEFAULT 0

LC3_ENCODER_CNTX lc3_enc_ctx;
LC3_DECODER_CNTX lc3_dec_ctx;

/* Source Buffer to be fed to LC3 Encoder Module */
static INT32  pcm_buffer[APPL_LC3_CHANNELS_MAX][LC3_INPUT_FRAME_SIZE_MAX];
/* LC3 Encoded Buffer Size */
static UINT8  enc_buffer[APPL_LC3_CHANNELS_MAX][LC3_FRAME_SIZE_MAX];
/* LC3 Decoded Buffer Size */
static INT32  dec_buffer[APPL_LC3_CHANNELS_MAX][LC3_INPUT_FRAME_SIZE_MAX];

/* Source Buffer Pointers holding pcm_buffer references */
static INT32* pcm_buf[APPL_LC3_CHANNELS_MAX];
/* LC3 Encoded Buffer Pointers holding enc_buffer references */
static UINT8* enc_buf[APPL_LC3_CHANNELS_MAX];
/* LC3 Decoded Buffer Pointers holding dec_buffer references */
static INT32* dec_buf[APPL_LC3_CHANNELS_MAX];

/* Core Buffer and Work Buffer to be fed to LC3 Encoder and Decoder Module */
static UINT8  enc_core_buffer[APPL_LC3_CHANNELS_MAX * LC3_ENCODER_CORE_BUFFER_SIZE_MAX];
static UINT8  enc_work_buffer[APPL_LC3_CHANNELS_MAX * LC3_ENCODER_WORK_BUFFER_SIZE_MAX];

static UINT8  dec_core_buffer[APPL_LC3_CHANNELS_MAX * LC3_DECODER_CORE_BUFFER_SIZE_MAX];
static UINT8  dec_work_buffer[APPL_LC3_CHANNELS_MAX * LC3_DECODER_WORK_BUFFER_SIZE_MAX];

#endif /* INCLUDE_ETHERMIND_LC3 */

#ifdef DUMP_ISO_DATA_TO_FILE
UINT8 is_dump_enabled;
FILE* lc3_enc_media_dump_fd;
FILE* lc3_dec_media_dump_fd;
FILE* lc3_dec_pre_media_dump_fd;
FILE* lc3_enc_pre_media_dump_fd;
FILE* lc3_enc_src_enqueue_fd;
FILE* lc3_audio_pl_src_callback_dump_fd;
#endif /* DUMP_ISO_DATA_TO_FILE */

#ifdef BAP_SUPPORT_CONTEXT_MANAGE
DECL_STATIC GA_BRR_SVC_RANGE appl_ga_svc_range[BAP_SERVICE_COUNT];
DECL_STATIC GA_CHAR_INFO * appl_ga_char_info;
DECL_STATIC GA_BRR_SVC_RANGE * appl_ga_svc_range_ptr;
DECL_STATIC GA_CHAR_INFO * appl_ga_char_info_ptr;
DECL_STATIC UINT16 appl_ga_char_count;
#endif /* BAP_SUPPORT_CONTEXT_MANAGE */

/* --------------------------------------------- Static Global Variables */
/* Copy the Hardcoded Peer Address */
/* UCHAR g_pts_bd_addr[6] = {0xE4, 0x3A, 0x92, 0xE9, 0xEC, 0xEF}; */

/* Laird Dongle PORT-13: F0 F3 FC B0 03 35 */
/* UCHAR g_pts_bd_addr[6] = {0x35, 0x03, 0xB0, 0xFC, 0xF3, 0xF0}; */

/* 4.0: China donge: 12 71 DA 7D 1A 00 */
/* UCHAR g_pts_bd_addr[6] = {0x12, 0x71, 0xDA, 0x7D, 0x1A, 0x00}; */

/* Laird Dongle PORT-16: 83 B3 E4 13 85 CA */
UCHAR g_pts_bd_addr[6] = {0x83, 0xB3, 0xE4, 0x13, 0x85, 0xCA};

/* 4.0 PTS Dongle */
/* UCHAR g_pts_bd_addr[6] = { 0x2C, 0x07, 0x06, 0xDC, 0x1B, 0x00}; */

/* --------------------------------------------- External Global Variables */

/*
 * To Validate if we are Initializing through GA_Init() in appl_ga.c
 * or appl_ga_legacy.c
 */
extern UINT8 ga_init_status;

/* --------------------------------------------- Exported Global Variables */
API_RESULT appl_ga_hci_event_callback(UINT8 event_code, UINT8 * event_data, UINT8 event_datalen)
{
    UINT16 acl_conn_handle, value_2, cis_conn_handle;
    UCHAR  status, value_1, cis_count, index;
    API_RESULT retval;
    UINT8 local_ase_db_index, local_ase_db_index_array[APPL_GA_MAX_ASE];
    UINT8 cig_id, cis_id, i;
    UINT8 ase_count;
    UINT8 reset;

    retval = API_FAILURE;
    local_ase_db_index = 0;
    ase_count = 0;
    i = 0;
    acl_conn_handle = 0;
    cis_conn_handle = 0;
    reset = GA_FALSE;

    /* Switch on the Event Code */
    switch (event_code)
    {
    case HCI_LE_META_EVENT:
        hci_unpack_1_byte_param(&value_1, event_data);
        event_data += 1;

        switch (value_1)
        {
        case HCI_LE_ENHANCED_CONNECTION_COMPLETE_SUBEVENT:
            /* Parse the Packet received to extract necessary information */
            hci_unpack_1_byte_param(&status, event_data);
            hci_unpack_2_byte_param(&acl_conn_handle, event_data + 1);
            if (0x00 == status)
            {
                if (APPL_GA_GATT_CLIENT == curr_GATT_role)
                {
                    ga_rem_dev_info[curr_rem_index].acl_conn_handle = acl_conn_handle;
#ifdef APPL_GA_CHAIN_CLIENT_PROCEDURES
                    /* Trigger Auth and Encrypt procedure */
                    appl_ga_unicast_auth_and_encrypt();

                    /* Trigger GA Procedures after a small timeout to allow MTU Exchange to finish */
                    BT_start_timer
                    (
                        &appl_ga_unicast_client_timer_handle,
                        3U,
                        appl_ga_unicast_client_timer_expiry_handler,
                        NULL,
                        0U
                    );

#endif /* APPL_GA_CHAIN_CLIENT_PROCEDURES */
                }
                else
                {
                    /* TODO */
                }
                hci_unpack_1_byte_param(&appl_ga_device.bd_type, event_data + 4);
                BT_COPY_BD_ADDR(appl_ga_device.bd_addr, event_data + 5);
            }
            break;

        case HCI_LE_CIS_REQUEST_SUBEVENT:
            hci_unpack_2_byte_param(&acl_conn_handle, event_data + 0);
            hci_unpack_2_byte_param(&cis_conn_handle, event_data + 2);
            hci_unpack_1_byte_param(&cig_id, event_data + 4);
            hci_unpack_1_byte_param(&cis_id, event_data + 5);


            APPL_TRC("cis_conn_handle: 0x%04X", cis_conn_handle);
            APPL_TRC("CIG ID: 0x%04X", cig_id);
            APPL_TRC("CIS ID: 0x%04X", cis_id);

            /* TODO: Check for the State */

            /*
             * Match the ASE ID with the requested CIG and CIS conn handle
             * rx during QoS Config. 2 different role ASE s can have same CIS ID
             * which translates to a Bidirectional CIS.
             */
            ase_count = 0;
            if (GA_FAILURE == appl_ga_fetch_local_ase_db_index_from_cig_cis_id(acl_conn_handle, cig_id, cis_id, local_ase_db_index_array, &ase_count))
            {
                printf("No registered ASEs connected with this ACL: 0x%04X, CIG: 0x%04X, CIS: 0x%04x\n", acl_conn_handle, cig_id, cis_id);
            }
            else
            {
                retval = BT_hci_le_accept_cis_request
                         (
                             cis_conn_handle
                         );

                if (API_SUCCESS != retval)
                {
                    APPL_TRC("FAILED !!! CIS Accepted Error Code = %04X\n", retval);
                }
                else
                {
                    APPL_TRC("CIS Accepted!\n");
                    for (index = 0; index < ase_count; index++)
                    {
                        local_ase_db_index = local_ase_db_index_array[index];
                        ga_local_audio_ase[local_ase_db_index].acl_conn_handle = acl_conn_handle;
                        ga_local_audio_ase[local_ase_db_index].cis_conn_handle = cis_conn_handle;
                    }
                }
            }
            break;

        case HCI_LE_CIS_ESTABLISHED_SUBEVENT:
            hci_unpack_1_byte_param(&status, event_data + 0);

            if (0 == status)
            {
                printf("CIS Established!!!\n");
                BT_debug_trace(BT_MODULE_ID_WT, "CIS Established!!!\n");
                hci_unpack_2_byte_param(&cis_conn_handle, event_data + 1);

                if (APPL_GA_GATT_SERVER == curr_GATT_role)
                {
                    ase_count = 0;
                    if (GA_FAILURE == appl_ga_fetch_local_ase_db_index_from_cis_conn_handle(cis_conn_handle, local_ase_db_index_array, &ase_count))
                    {
                        printf("No registered ASEs connected with this CIS 0x%04x", cis_conn_handle);
                        BT_debug_trace(BT_MODULE_ID_WT, "No registered ASEs connected with this CIS 0x%04x", cis_conn_handle);
                        /* TODO: Doesnt make sense below */
                        ga_local_audio_ase[local_ase_db_index].cis_conn_handle = 0xFFFF;
                    }
                    else
                    {
                        for (index = 0; index < ase_count; index++)
                        {
                            local_ase_db_index = local_ase_db_index_array[index];
                            APPL_TRC("local_ase_db_index: 0x%02x", local_ase_db_index);

                            appl_ga_prep_for_streaming(local_ase_db_index);
                        }
                    }
                }
                else
                {
                    if (GA_FAILURE == appl_ga_fetch_rem_ase_db_index_from_cis_conn_handle(cis_conn_handle, local_ase_db_index_array, &ase_count))
                    {
                        printf("No registered ASEs connected with this CIS 0x%04x", cis_conn_handle);
                        /* TODO: Doesnt make sense below */
                        ga_rem_dev_info[curr_rem_index].ga_rem_ase[local_ase_db_index].cis_conn_handle = 0xFFFF;
                    }
                    else
                    {
                        for (index = 0; index < ase_count; index++)
                        {
                            local_ase_db_index = local_ase_db_index_array[index];
                            APPL_TRC("local_ase_db_index: 0x%02x", local_ase_db_index);
                            appl_ga_prep_for_streaming(local_ase_db_index);
                        }
                    }
                }
            }
            else
            {
                printf("CIS Established Failed: 0x%02X!!!\n", status);
            }
            break;

        default:
            /* MISRA C-2012 Rule 16.4 */
            break;
        }
        break;

    case HCI_COMMAND_STATUS_EVENT:
        /* Status */
        hci_unpack_1_byte_param(&status, event_data);
        event_data += 1;

        /* Number of Command Packets */
        hci_unpack_1_byte_param(&value_1, event_data);
        event_data += 1;

        /* Command Opcode */
        hci_unpack_2_byte_param(&value_2, event_data);
        event_data += 2;

        switch (value_2)
        {
        case HCI_LE_CREATE_CIS_OPCODE:
            break;
        default:
            /* MISRA C-2012 Rule 16.4 */
            break;
        }
        break;

    case HCI_COMMAND_COMPLETE_EVENT:
        printf("Received HCI_COMMAND_COMPLETE_EVENT.\n");

        /* Number of Command Packets */
        hci_unpack_1_byte_param(&value_1, event_data);
        event_data += 1;

        /* Command Opcode */
        hci_unpack_2_byte_param(&value_2, event_data);
        event_data += 2;

        /* Command Status */
        hci_unpack_1_byte_param(&status, event_data);
        event_data += 1;

        /* Command Return Parameters */
        if (event_datalen > 4)
        {
            switch (value_2)
            {

            case HCI_LE_SET_HOST_FEATURE_OPCODE:
                break;

            case HCI_LE_SET_CIG_PARAMETERS_OPCODE:
                if (0x00 == status)
                {
                    hci_unpack_1_byte_param(&cig_id, event_data);
                    APPL_TRC("CIG ID: %d\n", cig_id);
                    event_data++;

                    hci_unpack_1_byte_param(&cis_count, event_data);
                    APPL_TRC("CIS Count: %d\n", cis_count);
                    event_data++;

                    /* TODO: Multiple CIS??? */
                    for (index = 0; index < cis_count; index++)
                    {
                        /* Connection Handle */
                        hci_unpack_2_byte_param(&cis_conn_handle, event_data);
                        APPL_TRC("CIS Conn Handle: 0x%04X\n", cis_conn_handle);

                        if (GA_FAILURE == appl_ga_fetch_rem_ase_db_index_from_cig_id(cig_id, &local_ase_db_index))
                        {
                            printf("Unknown CIG ID\n");
                        }
                        else
                        {
                            ga_rem_dev_info[curr_rem_index].ga_rem_ase[local_ase_db_index].cis_conn_handle = cis_conn_handle;
                            event_data += 2;
                        }
                    }

#ifdef APPL_GA_CHAIN_CLIENT_PROCEDURES
                    appl_ga_unicast_client_configure_qos();
#endif /* APPL_GA_CHAIN_CLIENT_PROCEDURES */
                }
                else
                {
                    /* TODO */
                }
                break;

            case HCI_LE_SETUP_ISO_DATA_PATH_OPCODE:
                if (0x00 == status)
                {
                    hci_unpack_2_byte_param(&cis_conn_handle, event_data);

                    if (APPL_GA_GATT_SERVER == curr_GATT_role)
                    {
                        ase_count = 0;
                        if (GA_FAILURE == appl_ga_fetch_local_ase_db_index_from_cis_conn_handle(cis_conn_handle, local_ase_db_index_array, &ase_count))
                        {
                            printf("No registered ASEs connected with this CIS 0x%04x", acl_conn_handle);
                        }
                        else
                        {
                            /* If Local Server is a Sink, then trigger Receiver Ready */
                            for (index = 0; index < ase_count; index++)
                            {
                                local_ase_db_index = local_ase_db_index_array[index];
                                APPL_TRC("ISO Data Setup Success, 0x%04x ASE_ID: 0x%02x\n", cis_conn_handle, local_ase_db_index);

                                if (GA_ROLE_SINK == ga_local_audio_ase[local_ase_db_index].codec_conf.role)
                                {
#if 1
                                    retval = GA_notify_ase_receiver_start_ready(&appl_ga_device, 1, &ga_local_audio_ase[local_ase_db_index].ase_id);

                                    if (GA_SUCCESS == retval)
                                    {
                                        ga_local_audio_ase[local_ase_db_index].curr_state = APPL_GA_STATE_STREAMING;
#ifdef APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA
                                        audio_start_pl(AUDIO_EP_SINK);
#endif /* APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA */
                                        isISODataPathReset = GA_TRUE;

#ifdef DUMP_ISO_DATA_TO_FILE
                                        if (GA_TRUE == is_dump_enabled)
                                        {
                                            lc3_dec_media_dump_fd = fopen("lc3_dec_media_dump", "wb+");

                                            if (NULL != lc3_dec_media_dump_fd)
                                            {
                                                APPL_TRC("Dec Media Dump created successfully\n");
                                            }

                                            lc3_dec_pre_media_dump_fd = fopen("lc3_dec_pre_media_dump", "wb+");

                                            if (NULL != lc3_dec_pre_media_dump_fd)
                                            {
                                                APPL_TRC("Pre Dec Media Dump created successfully\n");
                                            }
                                        }
#endif /* DUMP_ISO_DATA_TO_FILE */
                                    }
#endif
                                }
                            }
                        }
                    }
                    else if (APPL_GA_GATT_NONE == curr_GATT_role)
                    {
                        /* BIS Connection Handle */
                        APPL_TRC("ISO Data Setup Success, 0x%04x\n", cis_conn_handle);
                    }
                }
                break;

            case HCI_LE_REMOVE_ISO_DATA_PATH_OPCODE:
                if (0x00 == status)
                {
                    hci_unpack_2_byte_param(&cis_conn_handle, event_data);

                    if (APPL_GA_GATT_SERVER == curr_GATT_role)
                    {
                        if (GA_FAILURE == appl_ga_fetch_local_ase_db_index_from_cis_conn_handle(cis_conn_handle, local_ase_db_index_array, &ase_count))
                        {
                            printf("No registered ASEs connected with this CIS 0x%04x", cis_conn_handle);
                        }
                        else
                        {
                            for (index = 0; index < ase_count; index++)
                            {
                                local_ase_db_index = local_ase_db_index_array[index];
                                ga_local_audio_ase[local_ase_db_index].curr_state = APPL_GA_STATE_CODEC_CONF;
                            }
                        }
                    }
                    else
                    {
                        if (GA_FAILURE == appl_ga_fetch_rem_ase_db_index_from_cis_conn_handle(cis_conn_handle, local_ase_db_index_array, &ase_count))
                        {
                            printf("No registered ASEs connected with this CIS 0x%04x", cis_conn_handle);
                        }
                        else
                        {
                            for (index = 0; index < ase_count; index++)
                            {
                                local_ase_db_index = local_ase_db_index_array[index];
                                retval = BT_hci_le_remove_cig(ga_rem_dev_info[curr_rem_index].ga_rem_ase[local_ase_db_index].qos_conf.channel.cig_id);
                                if (API_SUCCESS == retval)
                                {
                                    APPL_TRC("CIS Disconnect Initiated\n");
                                }
                                else
                                {
                                    APPL_TRC("CIS Disconnect Initiate Failed 0x%02X!!!", retval);
                                }
                            }
                        }
                    }
                }
                break;

            default:
                /* MISRA C-2012 Rule 16.4 */
                break;
            }
        }
        break;

    case HCI_DISCONNECTION_COMPLETE_EVENT:
        /* Status */
        hci_unpack_1_byte_param(&status, event_data);
        printf("\tStatus: 0x%02X\n", status);
        event_data += 1;

        /* Connection Handle - CIS or ACL */
        hci_unpack_2_byte_param(&value_2, event_data);
        printf("\tConnection Handle: 0x%04X\n", value_2);
        event_data += 2;

        if (0x00 == status)
        {
            if (APPL_GA_GATT_SERVER == curr_GATT_role)
            {
                if (GA_SUCCESS == appl_ga_fetch_local_ase_db_index_from_cis_conn_handle(value_2, local_ase_db_index_array, &ase_count))
                {
                    reset = 2;
                }
                else if (GA_SUCCESS == appl_ga_fetch_local_ase_db_index_from_acl_conn_handle(value_2, local_ase_db_index_array, &ase_count))
                {
                    reset = 1;
                }
                else
                {
                    printf("No registered ASEs connected with this handle 0x%04x", value_2);
                }

                if (0 < reset)
                {
                    for (i = 0; i < ase_count; i++)
                    {
                        local_ase_db_index = local_ase_db_index_array[i];
                        if (APPL_GA_STATE_STREAMING == ga_local_audio_ase[local_ase_db_index].curr_state)
                        {
#ifdef APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA
                            if (GA_ROLE_SOURCE == ga_local_audio_ase[local_ase_db_index].codec_conf.role)
                            {
                                audio_stop_pl(AUDIO_EP_SOURCE);
                            }
                            else if (GA_ROLE_SINK == ga_local_audio_ase[local_ase_db_index].codec_conf.role)
                            {
                                audio_stop_pl(AUDIO_EP_SINK);
                            }
#endif /* APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA */

                            /* Remove reference to all the params w.r.t. this ASE */
                            ga_local_audio_ase[local_ase_db_index].cis_conn_handle = 0xFFFF;
                            ga_local_audio_ase[local_ase_db_index].curr_state = APPL_GA_STATE_REGISTERED;

                            if (1 == reset)
                            {
                                ga_local_audio_ase[local_ase_db_index].acl_conn_handle = 0xFFFF;
                            }
                        }
                    }
                }
            }
            else
            {
                if (GA_SUCCESS == appl_ga_fetch_rem_ase_db_index_from_cis_conn_handle(value_2, local_ase_db_index_array, &ase_count))
                {
                    reset = 2;
                }
                else if (GA_SUCCESS == appl_ga_fetch_rem_ase_db_index_from_acl_conn_handle(value_2, &local_ase_db_index))
                {
                    reset = 1;
                }
                else
                {
                    printf("No registered ASEs connected with this handle 0x%04x", value_2);
                }

                if (0 < reset)
                {
                    for (i = 0; i < ase_count; i++)
                    {
                        local_ase_db_index = local_ase_db_index_array[i];
                        ga_rem_dev_info[curr_rem_index].ga_rem_ase[local_ase_db_index].cis_conn_handle = 0xFFFF;
                        ga_rem_dev_info[curr_rem_index].ga_rem_ase[local_ase_db_index].curr_state = APPL_GA_STATE_REGISTERED;
                    }

                    if (1 == reset)
                    {
                        GA_mem_set(&ga_rem_dev_info[curr_rem_index], 0x00, sizeof(APPL_GA_REM_DEV_INFO));
                    }
                }
            }
        }

        break;

    default:
        /* MISRA C-2012 Rule 16.4 */
        break;
    }

    return API_SUCCESS;
}

GA_RESULT appl_ga_hci_iso_data_ind_cb
          (
              UCHAR* header,
              UCHAR* data,
              UINT16 datalen
          )
{
    GA_RESULT retval;

    /* TODO: Extract header if required with the ISO utility macros */
    GA_IGNORE_UNUSED_PARAM(header);

#ifndef INCLUDE_ETHERMIND_LC3
    GA_IGNORE_UNUSED_PARAM(data);
    GA_IGNORE_UNUSED_PARAM(datalen);
#endif /* INCLUDE_ETHERMIND_LC3 */

    retval = GA_FAILURE;

    /* APPL_TRC("ISO: LC3 Encoded Data : %d\n", datalen); */
    /* APPL_TRC("."); */

    if (GA_TRUE == appl_ga_lc3_dec_ready)
    {
#ifdef INCLUDE_ETHERMIND_LC3
        if ((0 < datalen) && (NULL != data))
        {
            /* Decode and send the rx packet */
            retval = appl_ga_lc3_decode_n_play(data, datalen);
        }
#endif /* INCLUDE_ETHERMIND_LC3 */
    }

    return retval;
}

GA_RESULT appl_bap_unicast_client_callback
          (
              GA_ENDPOINT * device,
              UINT8    ga_event,
              UINT16   ga_status,
              void   * ga_data,
              UINT16   ga_datalen
          )
{
    GA_RESULT     retval;

    UINT8 role;
    GA_CAP_ENDPOINT * capab;
    GA_ASE_INFO * ase;
    UINT8 metadata_len;
    GA_LC3_CODEC_SPECIFIC_CAP rem_cs_cap;
    GA_LC3_CODEC_SPECIFIC_CONF cs_conf;
    GA_CODEC_INFO              cs_conf_codec_id;
    UINT8                      marker;
    UINT8                      data_path_direction;
    UINT8                      local_ase_db_index;
    UINT8 rem_ase_index;
    GA_QOS_CONF qos_cfg;
    UCHAR               i;

    if (NULL == device)
    {
        printf("NULL Device Unexpected!\n");
        return GA_FAILURE;
    }

    GA_CB_ENTRY("Client");

    GA_IGNORE_UNUSED_PARAM(cs_conf);
    GA_IGNORE_UNUSED_PARAM(rem_cs_cap);

    /* Init */
    retval = GA_SUCCESS;
    i = 0;
    metadata_len = 0;
    marker = 0;
    data_path_direction = APPL_GA_ISO_UNKNOWN;
    local_ase_db_index = 0;
    rem_ase_index = 0;

    switch (ga_event)
    {
    case GA_SETUP_CNF:
        APPL_TRC("[Profile]     : PACS (0x%04X)\n", GA_SERVICE_PACS);
        APPL_TRC("[SECTION]     : PACS CLIENT EVENTS\n");
        APPL_TRC("[SUB-SECTION] : PACS CLIENT-SETUP\n");
        APPL_TRC("[TYPE]        : SETUP RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_ga_utils_display_ga_result(ga_status);
        APPL_TRC
        (
            "[Event]       : GA_SETUP_CNF (0x%02X)\n",
            ga_event
        );
        if (GA_SUCCESS == ga_status)
        {
            role = *((UINT8*)ga_data);
            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC("Data Length: 0x%02X\n", ga_datalen);
            APPL_TRC("Data:\n");
            APPL_TRC("Len: 0x%02X,  Supported ", (UCHAR)sizeof(UINT8));
            appl_ga_utils_bap_display_role(role);
#ifdef APPL_GA_CHAIN_CLIENT_PROCEDURES
            appl_ga_unicast_client_get_pacs_capabilities();
#endif /* APPL_GA_CHAIN_CLIENT_PROCEDURES */
        }
        break;

    case GA_RELEASE_CNF:
        APPL_TRC("[Profile]     : PACS (0x%04X)\n", GA_SERVICE_PACS);
        APPL_TRC("[SECTION]     : PACS CLIENT EVENTS\n");
        APPL_TRC("[SUB-SECTION] : PACS CLIENT-RELEASE\n");
        APPL_TRC("[TYPE]        : RELEASE RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_ga_utils_display_ga_result(ga_status);
        APPL_TRC
        (
            "[Event]       : GA_RELEASE_CNF (0x%02X)\n",
            ga_event
        );
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%02X\n", ga_datalen);
        if (0U == ga_datalen)
        {
            APPL_TRC("Data: NULL\n");
        }
        break;

    case GA_GET_CAPABILITIES_CNF:
        capab = (GA_CAP_ENDPOINT*)ga_data;

        APPL_TRC("[Profile]     : PACS (0x%04X)\n", GA_SERVICE_PACS);
        APPL_TRC("[SECTION]     : PACS CLIENT EVENTS\n");
        APPL_TRC("[SUB-SECTION] : PACS CLIENT-SETUP\n");
        APPL_TRC("[TYPE]        : SETUP RESPONSE\n");
        appl_ga_utils_display_bdaddr_and_type(device);
        APPL_TRC("[Event Status]: ");
        appl_ga_utils_display_ga_result(ga_status);
        APPL_TRC
        (
            "[Event]       : GA_GET_CAPABILITIES_CNF (0x%02X)\n",
            ga_event
        );

        retval = appl_ga_utils_bap_add_remote_pacs_for_dev
                 (
                     device,
                     capab
                 );

        marker = 0;

        if (GA_CAP_TYPE_CODECS == capab->type)
        {
            /* No of PAC Records */
            marker++;
            for (i = 0; i < capab->data[0]; i++)
            {
                GA_UNPACK_LE_1_BYTE
                (
                    &ga_rem_dev_info[curr_rem_index].ga_rem_audio_cap[ga_rem_dev_info[curr_rem_index].ga_rem_audio_cap_count].codec.coding_format,
                    &capab->data[marker]
                );
                marker += 1;

                GA_UNPACK_LE_2_BYTE
                (
                    &ga_rem_dev_info[curr_rem_index].ga_rem_audio_cap[ga_rem_dev_info[curr_rem_index].ga_rem_audio_cap_count].codec.company_id,
                    &capab->data[marker]
                );
                marker += 2;

                GA_UNPACK_LE_2_BYTE
                (
                    &ga_rem_dev_info[curr_rem_index].ga_rem_audio_cap[ga_rem_dev_info[curr_rem_index].ga_rem_audio_cap_count].codec.vendor_codec_id,
                    &capab->data[marker]
                );
                marker += 2;

                ga_rem_dev_info[curr_rem_index].ga_rem_audio_cap[ga_rem_dev_info[curr_rem_index].ga_rem_audio_cap_count].codec.ie_len = capab->data[marker];
                marker++;

                GA_mem_copy(&ga_rem_dev_info[curr_rem_index].ga_rem_audio_cap[ga_rem_dev_info[curr_rem_index].ga_rem_audio_cap_count].codec.ie, &capab->data[marker], ga_rem_dev_info[curr_rem_index].ga_rem_audio_cap[ga_rem_dev_info[curr_rem_index].ga_rem_audio_cap_count].codec.ie_len);
                marker += ga_rem_dev_info[curr_rem_index].ga_rem_audio_cap[ga_rem_dev_info[curr_rem_index].ga_rem_audio_cap_count].codec.ie_len;

                metadata_len = capab->data[marker];
                marker++;
                marker += metadata_len;

                ga_rem_dev_info[curr_rem_index].ga_rem_audio_cap_count++;
            }
        }
        else if (GA_CAP_TYPE_LOCATIONS == capab->type)
        {
            if (0x04 == capab->datalen)
            {
                if (GA_ROLE_SINK & capab->role)
                {
                    GA_mem_copy(&ga_rem_dev_info[curr_rem_index].ga_rem_pacs.sink_audio_locations, capab->data, capab->datalen);
                }
                else if (GA_ROLE_SOURCE & capab->role)
                {
                    GA_mem_copy(&ga_rem_dev_info[curr_rem_index].ga_rem_pacs.src_audio_locations, capab->data, capab->datalen);
                }
            }
            else
            {
                printf("\tIncorrect Length!!!\n");
                printf("\tLength - 0x%02X\n", capab->datalen);
                printf("\tData:\n");
                appl_dump_bytes(capab->data, capab->datalen);
            }
        }
        else if (GA_CAP_TYPE_AVAIL_CONTEXTS == capab->type)
        {
            if (0x04 == capab->datalen)
            {
                GA_UNPACK_LE_2_BYTE
                (
                    &ga_rem_dev_info[curr_rem_index].ga_rem_pacs.avail_sink_contexts,
                    capab->data
                );
                if (0x0000 == ga_rem_dev_info[curr_rem_index].ga_rem_pacs.avail_sink_contexts)
                {
                    printf("\tServer is not available to receive audio for any Context Type value.\n");
                }

                GA_UNPACK_LE_2_BYTE
                (
                    &ga_rem_dev_info[curr_rem_index].ga_rem_pacs.avail_src_contexts,
                    capab->data + 2U
                );
                if (0x0000 == ga_rem_dev_info[curr_rem_index].ga_rem_pacs.avail_src_contexts)
                {
                    printf("\tServer is not available to transmit audio for any Context Type value.\n");
                }
#ifdef APPL_GA_CHAIN_CLIENT_PROCEDURES
                appl_ga_unicast_client_discover_ascs();
#endif /* APPL_GA_CHAIN_CLIENT_PROCEDURES */
            }
            else
            {
                printf("\tIncorrect Length!!!\n");
                printf("\tLength - 0x%02X\n", capab->datalen);
                printf("\tData:\n");
                appl_dump_bytes(capab->data, capab->datalen);
            }
        }
        else if (GA_CAP_TYPE_SUPP_CONTEXTS == capab->type)
        {
            if (0x04 == capab->datalen)
            {
                GA_UNPACK_LE_2_BYTE
                (
                    &ga_rem_dev_info[curr_rem_index].ga_rem_pacs.supp_sink_contexts,
                    capab->data
                );
                if (0x0000 == ga_rem_dev_info[curr_rem_index].ga_rem_pacs.supp_sink_contexts)
                {
                    printf("\tServer is not available to receive audio for any Context Type value.\n");
                }

                GA_UNPACK_LE_2_BYTE
                (
                    &ga_rem_dev_info[curr_rem_index].ga_rem_pacs.supp_src_contexts,
                    capab->data + 2U
                );
                if (0x0000 == ga_rem_dev_info[curr_rem_index].ga_rem_pacs.supp_src_contexts)
                {
                    printf("\tServer is not available to transmit audio for any Context Type value.\n");
                }
            }
            else
            {
                printf("\tIncorrect Length!!!\n");
                printf("\tLength - 0x%02X\n", capab->datalen);
                printf("\tData:\n");
                appl_dump_bytes(capab->data, capab->datalen);
            }
        }
        else
        {
            printf("Unknown Capability Type!!!\n");
        }
        break;

    case GA_SEP_DISCOVER_CNF:
        ase = (GA_ASE_INFO*)ga_data;

        appl_ga_utils_ucc_ascs_display_common_event_info
        (
            device,
            ga_event,
            ga_status
        );
        APPL_TRC
        (
            "[Event]       : GA_SEP_DISCOVER_CNF (0x%02X)\n",
            ga_event
        );

        APPL_TRC("\n");
        if (GA_ROLE_SINK & ase->ase_role)
        {
            appl_ga_utils_ascs_display_char_uuid(GA_CHAR_ASCS_SINK_ASE);
        }
        else if (GA_ROLE_SOURCE & ase->ase_role)
        {
            appl_ga_utils_ascs_display_char_uuid(GA_CHAR_ASCS_SOURCE_ASE);
        }
        if ((GA_SUCCESS == ga_status) || (GA_CONTINUE == ga_status))
        {
            APPL_TRC("SEP Context ID - 0x%02X\n", ase->sep_id);
            appl_ga_utils_bap_display_role(ase->ase_role);
            APPL_TRC("Data Length: 0x%02X\n", ase->datalen);
            APPL_TRC("Data:\n");
            appl_ga_utils_ucc_ascs_parse_and_display_ase_data
            (
                ase->data,
                ase->datalen
            );

            appl_ga_fetch_rem_free_ase_db_index(&rem_ase_index);
            ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].ase_id = ase->data[0];
            ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].ase_role = ase->ase_role;
        }

#ifdef APPL_GA_CHAIN_CLIENT_PROCEDURES
        appl_ga_unicast_client_configure_codec();
#endif /* APPL_GA_CHAIN_CLIENT_PROCEDURES */
        break;

    case GA_SEP_CONFIGURE_CNF:
        ase = (GA_ASE_INFO*)ga_data;

        appl_ga_utils_ucc_ascs_display_common_event_info
        (
            device,
            ga_event,
            ga_status
        );
        APPL_TRC
        (
            "[Event]       : GA_SEP_CONFIGURE_CNF (0x%02X)\n",
            ga_event
        );

        appl_ga_utils_ucc_ascs_display_cp_event_data(ga_status, ase);
        if (GA_SUCCESS == ga_status)
        {
            /*
             * Preferred QoS is made available here, Mandatorily this needs to be
             * used during GA_sep_setup().
             */
            marker = 1;
            if (0x01 == ase->data[marker++])
            {
                appl_ga_fetch_rem_ase_db_index_from_ase_id(ase->data[0], &rem_ase_index);

                ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].qos_cap.pref_framing = ase->data[marker++];

                ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].qos_cap.pref_phy = ase->data[marker++];

                ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].qos_cap.pref_retxn = ase->data[marker++];

                GA_UNPACK_LE_2_BYTE(&ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].qos_cap.pref_max_transport_latency, &ase->data[marker]);
                marker += 2;

                GA_UNPACK_LE_3_BYTE(&ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].qos_cap.supp_presentation_delay_min, &ase->data[marker]);
                marker += 3;

                GA_UNPACK_LE_3_BYTE(&ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].qos_cap.supp_presentation_delay_max, &ase->data[marker]);
                marker += 3;

                GA_UNPACK_LE_3_BYTE(&ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].qos_cap.pref_presentation_delay_min, &ase->data[marker]);
                marker += 3;

                GA_UNPACK_LE_3_BYTE(&ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].qos_cap.pref_presentation_delay_max, &ase->data[marker]);
                marker += 3;

                GA_UNPACK_LE_1_BYTE(&cs_conf_codec_id.coding_format, &ase->data[marker]);
                marker += 1;

                GA_UNPACK_LE_2_BYTE(&cs_conf_codec_id.company_id, &ase->data[marker]);
                marker += 2;

                GA_UNPACK_LE_2_BYTE(&cs_conf_codec_id.vendor_codec_id, &ase->data[marker]);
                marker += 2;

                /* Codec_Specific_Configuration_Length */
                marker++;
                marker += ase->data[(marker - 1)];

                /**
                 * Set CIG Parameters using above qos_cap exposed.
                 * Copy qos_cap to qos_cfg to be used during Config QoS.
                 */

                 /* CIG ID can be any value decided by GA Appl */
                ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].qos_conf.framing = ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].qos_cap.pref_framing;
                ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].qos_conf.max_transport_latency = ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].qos_cap.pref_max_transport_latency;
                ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].qos_conf.phy = ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].qos_cap.pref_phy;
                ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].qos_conf.presentation_delay = ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].qos_cap.pref_presentation_delay_max;
                ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].qos_conf.retxn = ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].qos_cap.pref_retxn;

                /* Use GA App decided parameters */
                ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].qos_conf.sdu_interval = APPL_GA_SDU_INTERVAL;

                ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].bytes_to_lc3_encode =
                    ((APPL_GA_CODEC_SUPP_SAMPL_FREQ * APPL_GA_CODEC_SUPP_FRAME_DURATION_MS * APPL_GA_ASE_CHANNEL_COUNT * 2) /
                        (10 * 1000));
                /**
                 * Denominator * 1000 -> APPL_GA_CODEC_SUPP_SAMPL_FREQ 16000 to be fed here as 16
                 * and APPL_GA_CODEC_SUPP_FRAME_DURATION_MS to be either 7.5 or 10 based on the value
                 * set.
                 */

                 /*
                 appl_ga_curr_src_ase.qos_cfg.max_transport_latency = 8;
                 appl_ga_curr_src_ase.qos_cfg.max_sdu = 0x64;
                 appl_ga_curr_src_ase.qos_cfg.presentation_delay = 40000;
                 */

                 /* TODO: Runtime Computation based on the received QoS params from remote */
                ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].qos_conf.max_sdu = APPL_GA_CODEC_MAX_SDU; /* appl_ga_curr_src_ase.lc3_encoded_bytes; */
                ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].sca = APPL_GA_ASE_SCA;
                ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].packing = APPL_GA_ASE_PACKING_SEQUENTIAL;

                /* TODO: Set CIG Params */

                ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].curr_state = APPL_GA_STATE_CODEC_CONF;

#ifdef APPL_GA_CHAIN_CLIENT_PROCEDURES
                appl_ga_unicast_client_setup_cig();
#endif /* APPL_GA_CHAIN_CLIENT_PROCEDURES */
            }
        }
        break;

    case GA_SEP_SETUP_CNF:
        ase = (GA_ASE_INFO*)ga_data;

        appl_ga_utils_ucc_ascs_display_common_event_info
        (
            device,
            ga_event,
            ga_status
        );
        APPL_TRC
        (
            "[Event]       : GA_SEP_SETUP_CNF (0x%02X)\n",
            ga_event
        );

        appl_ga_utils_ucc_ascs_display_cp_event_data(ga_status, ase);
        if (GA_SUCCESS == ga_status)
        {
            if (0x02 == ase->data[1])
            {
                qos_cfg.channel.cig_id = ase->data[2];

                qos_cfg.channel.cis_id = ase->data[3];

                GA_UNPACK_LE_3_BYTE(&qos_cfg.sdu_interval, &ase->data[4]);

                qos_cfg.framing = ase->data[7];

                qos_cfg.phy = ase->data[8];

                GA_UNPACK_LE_2_BYTE(&qos_cfg.max_sdu, &ase->data[9]);

                qos_cfg.retxn = ase->data[11];

                GA_UNPACK_LE_2_BYTE
                (
                    &qos_cfg.max_transport_latency,
                    &ase->data[12]
                );

                GA_UNPACK_LE_3_BYTE
                (
                    &qos_cfg.presentation_delay,
                    &ase->data[14]
                );

                /* TODO: Create CIS, Post that Set ISO Data path */

                ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].curr_state = APPL_GA_STATE_QOS_CONF;
            }


#ifdef APPL_GA_CHAIN_CLIENT_PROCEDURES
            appl_ga_unicast_client_enable();
#endif /* APPL_GA_CHAIN_CLIENT_PROCEDURES */
        }
        break;

    case GA_SEP_START_CNF:
        ase = (GA_ASE_INFO*)ga_data;

        appl_ga_utils_ucc_ascs_display_common_event_info
        (
            device,
            ga_event,
            ga_status
        );
        APPL_TRC
        (
            "[Event]       : GA_SEP_START_CNF (0x%02X)\n",
            ga_event
        );

        appl_ga_utils_ucc_ascs_display_cp_event_data(ga_status, ase);
        if (GA_SUCCESS == ga_status)
        {
            if (GA_FAILURE == appl_ga_fetch_rem_ase_db_index_from_ase_id(ase->data[0], &rem_ase_index))
            {
                printf("Unknown ASE ID\n");
            }
            else
            {
                ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].curr_state = APPL_GA_STATE_ENABLING;

#ifdef APPL_GA_CHAIN_CLIENT_PROCEDURES
            appl_ga_unicast_client_create_cis();
#endif /* APPL_GA_CHAIN_CLIENT_PROCEDURES */
            }
        }
        break;

    case GA_SEP_SUSPEND_CNF:
        ase = (GA_ASE_INFO*)ga_data;

        appl_ga_utils_ucc_ascs_display_common_event_info
        (
            device,
            ga_event,
            ga_status
        );
        APPL_TRC
        (
            "[Event]       : GA_SEP_SUSPEND_CNF (0x%02X)\n",
            ga_event
        );

        appl_ga_utils_ucc_ascs_display_cp_event_data(ga_status, ase);
        if (GA_SUCCESS == ga_status)
        {
            if (GA_FAILURE == appl_ga_fetch_rem_ase_db_index_from_ase_id(ase->data[0], &rem_ase_index))
            {
                printf("Unknown ASE ID\n");
            }
            else
            {
                if (GA_ROLE_SINK == ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].ase_role)
                {
                    ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].curr_state = APPL_GA_STATE_DISABLING;
#ifdef APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA
                    audio_stop_pl(AUDIO_EP_SOURCE);
#endif /* APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA */
                }
                else
                {
                    ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].curr_state = APPL_GA_STATE_QOS_CONF;
#ifdef APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA
                    audio_stop_pl(AUDIO_EP_SINK);
#endif /* APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA */
                }
            }
        }
        break;

    case GA_SEP_RX_START_READY_CNF:
        ase = (GA_ASE_INFO*)ga_data;

        appl_ga_utils_ucc_ascs_display_common_event_info
        (
            device,
            ga_event,
            ga_status
        );
        APPL_TRC
        (
            "[Event]       : GA_SEP_RX_START_READY_CNF (0x%02X)\n",
            ga_event
        );

        appl_ga_utils_ucc_ascs_display_cp_event_data(ga_status, ase);
        if (GA_SUCCESS == ga_status)
        {
            retval = appl_ga_fetch_rem_ase_db_index_from_ase_id(ase->data[0], &local_ase_db_index);
            if (GA_SUCCESS != retval)
            {
                printf("Unknown ASE ID\n");
            }
            else
            {
                if ((0x04 == ase->data[1])
                    &&
                    (APPL_GA_STATE_ENABLING == ga_rem_dev_info[curr_rem_index].ga_rem_ase[local_ase_db_index].curr_state))
                {
                    ga_rem_dev_info[curr_rem_index].ga_rem_ase[local_ase_db_index].curr_state = APPL_GA_STATE_STREAMING;
                    if (GA_ROLE_SOURCE == ga_rem_dev_info[curr_rem_index].ga_rem_ase[local_ase_db_index].ase_role)
                    {
                        if (APPL_GA_GATT_CLIENT == curr_GATT_role)
                        {
                            /* We are Client Sink, Be ready to RX Data */
#ifdef APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA
                            audio_start_pl(AUDIO_EP_SINK);
#endif /* APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA */

                            isISODataPathReset = GA_TRUE;

#ifdef DUMP_ISO_DATA_TO_FILE
                        if (GA_TRUE == is_dump_enabled)
                        {
                            lc3_dec_media_dump_fd = fopen("lc3_dec_media_dump", "wb+");

                            if (NULL != lc3_dec_media_dump_fd)
                            {
                                APPL_TRC("Dec Media Dump created successfully\n");
                            }

                            lc3_dec_pre_media_dump_fd = fopen("lc3_dec_pre_media_dump", "wb+");

                            if (NULL != lc3_dec_pre_media_dump_fd)
                            {
                                APPL_TRC("Dec Media Dump created successfully\n");
                            }
                        }
#endif /* DUMP_ISO_DATA_TO_FILE */
                    }
                }
                else
                {
                    /* We are Client Source, Be ready to TX Data */
#ifdef APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA

                    audio_start_pl(AUDIO_EP_SOURCE);
                    ga_local_audio_src_streaming = GA_TRUE;

#ifdef DUMP_ISO_DATA_TO_FILE
                    if (GA_TRUE == is_dump_enabled)
                    {
                        lc3_enc_media_dump_fd = fopen("lc3_enc_media_dump", "wb+");
                        if (NULL != lc3_enc_media_dump_fd)
                        {
                            APPL_TRC("Enc Media Dump created successfully\n");
                        }

                        lc3_enc_pre_media_dump_fd = fopen("lc3_enc_pre_media_dump", "wb+");
                        if (NULL != lc3_enc_pre_media_dump_fd)
                        {
                            APPL_TRC("Enc Pre Media Dump created successfully\n");
                        }

                        lc3_enc_src_enqueue_fd = fopen("lc3_enc_src_enqueue_dump", "wb+");
                        if (NULL != lc3_enc_src_enqueue_fd)
                        {
                            APPL_TRC(
                                "Src Enqueue Media Dump created successfully\n");
                        }

                        lc3_audio_pl_src_callback_dump_fd = fopen("lc3_audio_pl_src_callback_dump", "wb+");
                        if (NULL != lc3_audio_pl_src_callback_dump_fd)
                        {
                            APPL_TRC(
                                "Audio Pl Src Callback Media Dump created successfully\n");
                        }
                    }
#endif /* DUMP_ISO_DATA_TO_FILE */
#endif /* APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA */
                    }
                }
            }
        }
        break;

    case GA_SEP_RX_STOP_READY_CNF:
        ase = (GA_ASE_INFO*)ga_data;

        appl_ga_utils_ucc_ascs_display_common_event_info
        (
            device,
            ga_event,
            ga_status
        );
        APPL_TRC
        (
            "[Event]       : GA_SEP_RX_STOP_READY_CNF (0x%02X)\n",
            ga_event
        );

        appl_ga_utils_ucc_ascs_display_cp_event_data(ga_status, ase);
        if (GA_SUCCESS == ga_status)
        {
            if (GA_FAILURE == appl_ga_fetch_rem_ase_db_index_from_ase_id(ase->data[0], &rem_ase_index))
            {
                printf("Unknown ASE ID\n");
            }
            else
            {
                ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].curr_state = APPL_GA_STATE_SUSPENDED;
            }
        }
        break;

    case GA_SEP_UPDATE_CNF:
        ase = (GA_ASE_INFO*)ga_data;

        appl_ga_utils_ucc_ascs_display_common_event_info
        (
            device,
            ga_event,
            ga_status
        );
        APPL_TRC
        (
            "[Event]       : GA_SEP_RX_STOP_READY_CNF (0x%02X)\n",
            ga_event
        );

        appl_ga_utils_ucc_ascs_display_cp_event_data(ga_status, ase);
        break;

    case GA_SEP_RELEASE_CNF:
        ase = (GA_ASE_INFO*)ga_data;

        appl_ga_utils_ucc_ascs_display_common_event_info
        (
            device,
            ga_event,
            ga_status
        );
        APPL_TRC
        (
            "[Event]       : GA_SEP_RELEASE_CNF (0x%02X)\n",
            ga_event
        );

        appl_ga_utils_ucc_ascs_display_cp_event_data(ga_status, ase);
        if (GA_SUCCESS == ga_status)
        {
            if (GA_SUCCESS == ga_status)
            {
                if (GA_FAILURE == appl_ga_fetch_rem_ase_db_index_from_ase_id(ase->data[0], &rem_ase_index))
                {
                    printf("Unknown ASE ID\n");
                }
                else
                {
                    ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].curr_state = APPL_GA_STATE_SUSPENDED;

                    if (GA_ROLE_SINK == ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].ase_role)
                    {
                        data_path_direction = APPL_GA_ISO_INPUT;
                    }
                    else if (GA_ROLE_SOURCE == ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].ase_role)
                    {
                        data_path_direction = APPL_GA_ISO_OUTPUT;
                    }

                    APPL_TRC(
                        "BT_hci_le_remove_iso_data_path data_path_direction %d\n",
                        data_path_direction);
                    retval = BT_hci_le_remove_iso_data_path
                    (
                        ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].cis_conn_handle,
                        data_path_direction
                    );

                    if (API_SUCCESS != retval)
                    {
                        APPL_TRC("FAILED !!! Error code = %04X\n", retval);
                    }
                    else
                    {
                        APPL_TRC("API returned success...\n");

#ifdef APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA
                        if (APPL_GA_ISO_INPUT == data_path_direction)
                        {
                            /* Setup Module for Media Data - Generate Audio Data */
                            audio_shutdown_pl();

#ifdef INCLUDE_ETHERMIND_LC3
                            /* De-init LC3 Module */
                            LC3_encoder_delete(&lc3_enc_ctx);
                            appl_ga_lc3_enc_ready = GA_FALSE;
#endif /* INCLUDE_ETHERMIND_LC3 */
                        }
                        else if (APPL_GA_ISO_OUTPUT == data_path_direction)
                        {
                            /* Setup Module for Media Data - Play Audio Data */
                            audio_shutdown_pl();

#ifdef INCLUDE_ETHERMIND_LC3
                            /* De-init LC3 Module */
                            LC3_decoder_delete(&lc3_dec_ctx);
                            appl_ga_lc3_dec_ready = GA_FALSE;
#endif /* INCLUDE_ETHERMIND_LC3 */
                        }
#endif /* APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA */
                    }
                }
            }
        }
        break;

    case GA_SEP_CONFIGURE_NTF:

        ase = (GA_ASE_INFO*)ga_data;

        appl_ga_utils_ucc_ascs_display_common_event_info
        (
            device,
            ga_event,
            ga_status
        );
        APPL_TRC
        (
            "[Event]       : GA_SEP_CONFIGURE_NTF (0x%02X)\n",
            ga_event
        );

        appl_ga_utils_ucc_ascs_display_cp_event_data(ga_status, ase);

        /*
         * Preferred QoS is made available here, Mandatorily this needs to be
         * used during GA_sep_setup().
         */
        if (GA_SUCCESS == ga_status)
        {
            marker = 1;
            if (0x01 == ase->data[marker++])
            {
                appl_ga_fetch_rem_ase_db_index_from_ase_id(ase->data[0], &rem_ase_index);

                ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].qos_cap.pref_framing = ase->data[marker++];

                ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].qos_cap.pref_phy = ase->data[marker++];

                ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].qos_cap.pref_retxn = ase->data[marker++];

                GA_UNPACK_LE_2_BYTE(&ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].qos_cap.pref_max_transport_latency, &ase->data[marker]);
                marker += 2;

                GA_UNPACK_LE_3_BYTE(&ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].qos_cap.supp_presentation_delay_min, &ase->data[marker]);
                marker += 3;

                GA_UNPACK_LE_3_BYTE(&ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].qos_cap.supp_presentation_delay_max, &ase->data[marker]);
                marker += 3;

                GA_UNPACK_LE_3_BYTE(&ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].qos_cap.pref_presentation_delay_min, &ase->data[marker]);
                marker += 3;

                GA_UNPACK_LE_3_BYTE(&ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].qos_cap.pref_presentation_delay_max, &ase->data[marker]);
                marker += 3;

                GA_UNPACK_LE_1_BYTE(&cs_conf_codec_id.coding_format, &ase->data[marker]);
                marker += 1;

                GA_UNPACK_LE_2_BYTE(&cs_conf_codec_id.company_id, &ase->data[marker]);
                marker += 2;

                GA_UNPACK_LE_2_BYTE(&cs_conf_codec_id.vendor_codec_id, &ase->data[marker]);
                marker += 2;

                /* Codec_Specific_Configuration_Length */
                marker++;
                marker += ase->data[(marker - 1)];

                /**
                 * Set CIG Parameters using above qos_cap exposed.
                 * Copy qos_cap to qos_cfg to be used during Config QoS.
                 */

                 /* CIG ID can be any value decided by GA Appl */
                ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].qos_conf.framing = ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].qos_cap.pref_framing;
                ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].qos_conf.max_transport_latency = ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].qos_cap.pref_max_transport_latency;
                ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].qos_conf.phy = ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].qos_cap.pref_phy;
                ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].qos_conf.presentation_delay = ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].qos_cap.pref_presentation_delay_max;
                ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].qos_conf.retxn = ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].qos_cap.pref_retxn;

                /* Use GA App decided parameters */
                ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].qos_conf.sdu_interval = APPL_GA_SDU_INTERVAL;

                ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].bytes_to_lc3_encode =
                    ((APPL_GA_CODEC_SUPP_SAMPL_FREQ * APPL_GA_CODEC_SUPP_FRAME_DURATION_MS * APPL_GA_ASE_CHANNEL_COUNT * 2) /
                        (10 * 1000));
                /**
                 * Denominator * 1000 -> APPL_GA_CODEC_SUPP_SAMPL_FREQ 16000 to be fed here as 16
                 * and APPL_GA_CODEC_SUPP_FRAME_DURATION_MS to be either 7.5 or 10 based on the value
                 * set.
                 */

                 /*
                 appl_ga_curr_src_ase.qos_cfg.max_transport_latency = 8;
                 appl_ga_curr_src_ase.qos_cfg.max_sdu = 0x64;
                 appl_ga_curr_src_ase.qos_cfg.presentation_delay = 40000;
                 */

                 /* TODO: Runtime Computation based on the received QoS params from remote */
                ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].qos_conf.max_sdu = APPL_GA_CODEC_MAX_SDU; /* appl_ga_curr_src_ase.lc3_encoded_bytes; */
                ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].sca = APPL_GA_ASE_SCA;
                ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].packing = APPL_GA_ASE_PACKING_SEQUENTIAL;

                /* TODO: Set CIG Params */

                ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].curr_state = APPL_GA_STATE_CODEC_CONF;
            }
        }
        break;

    case GA_SEP_SETUP_NTF:

        ase = (GA_ASE_INFO*)ga_data;

        appl_ga_utils_ucc_ascs_display_common_event_info
        (
            device,
            ga_event,
            ga_status
        );
        APPL_TRC
        (
            "[Event]       : GA_SEP_SETUP_NTF (0x%02X)\n",
            ga_event
        );

        appl_ga_utils_ucc_ascs_display_cp_event_data(ga_status, ase);

        if (0x02 == ase->data[1])
        {
            qos_cfg.channel.cig_id = ase->data[2];

            qos_cfg.channel.cis_id = ase->data[3];

            GA_UNPACK_LE_3_BYTE(&qos_cfg.sdu_interval, &ase->data[4]);

            qos_cfg.framing = ase->data[7];

            qos_cfg.phy = ase->data[8];

            GA_UNPACK_LE_2_BYTE(&qos_cfg.max_sdu, &ase->data[9]);

            qos_cfg.retxn = ase->data[11];

            GA_UNPACK_LE_2_BYTE
            (
                &qos_cfg.max_transport_latency,
                &ase->data[12]
            );

            GA_UNPACK_LE_3_BYTE
            (
                &qos_cfg.presentation_delay,
                &ase->data[14]
            );

            /* TODO: Create CIS, Post that Set ISO Data path */

            ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].curr_state = APPL_GA_STATE_QOS_CONF;
        }

        if (GA_FAILURE == appl_ga_fetch_rem_ase_db_index_from_ase_id(ase->data[0], &rem_ase_index))
        {
            printf("Unknown ASE ID\n");
        }
        else
        {
            if (GA_ROLE_SINK == ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].ase_role)
            {
                ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].curr_state = APPL_GA_STATE_DISABLING;
#ifdef APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA
                audio_stop_pl(AUDIO_EP_SOURCE);
#endif /* APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA */
            }
            else
            {
                ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].curr_state = APPL_GA_STATE_QOS_CONF;
#ifdef APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA
                audio_stop_pl(AUDIO_EP_SINK);
#endif /* APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA */
            }
        }
        break;

    case GA_SEP_SUSPEND_NTF:
        ase = (GA_ASE_INFO*)ga_data;

        appl_ga_utils_ucc_ascs_display_common_event_info
        (
            device,
            ga_event,
            ga_status
        );
        APPL_TRC
        (
            "[Event]       : GA_SEP_SUSPEND_NTF (0x%02X)\n",
            ga_event
        );

        appl_ga_utils_ucc_ascs_display_cp_event_data(ga_status, ase);

        if (GA_FAILURE == appl_ga_fetch_rem_ase_db_index_from_ase_id(ase->data[0], &rem_ase_index))
        {
            printf("Unknown ASE ID\n");
        }
        else
        {
            if (GA_ROLE_SINK == ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].ase_role)
            {
                ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].curr_state = APPL_GA_STATE_DISABLING;
#ifdef APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA
                audio_stop_pl(AUDIO_EP_SOURCE);
#endif /* APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA */
            }
            else
            {
                ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].curr_state = APPL_GA_STATE_QOS_CONF;
#ifdef APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA
                audio_stop_pl(AUDIO_EP_SINK);
#endif /* APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA */
            }
        }
        break;

    case GA_SEP_UPDATE_NTF:

        ase = (GA_ASE_INFO*)ga_data;
        appl_ga_utils_ucc_ascs_display_common_event_info
        (
            device,
            ga_event,
            ga_status
        );
        APPL_TRC
        (
            "[Event]       : GA_SEP_UPDATE_NTF (0x%02X)\n",
            ga_event
        );

        appl_ga_utils_ucc_ascs_display_cp_event_data(ga_status, ase);
        break;

    case GA_SEP_RELEASE_NTF:

        ase = (GA_ASE_INFO*)ga_data;
        appl_ga_utils_ucc_ascs_display_common_event_info
        (
            device,
            ga_event,
            ga_status
        );
        APPL_TRC
        (
            "[Event]       : GA_SEP_RELEASE_NTF (0x%02X)\n",
            ga_event
        );

        appl_ga_utils_ucc_ascs_display_cp_event_data(ga_status, ase);
        if (GA_SUCCESS == ga_status)
        {
            if (GA_FAILURE == appl_ga_fetch_rem_ase_db_index_from_ase_id(ase->data[0], &rem_ase_index))
            {
                printf("Unknown ASE ID\n");
            }
            else
            {
                ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].curr_state = APPL_GA_STATE_SUSPENDED;

                if (GA_ROLE_SINK == ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].ase_role)
                {
                    data_path_direction = APPL_GA_ISO_INPUT;
                }
                else if (GA_ROLE_SOURCE == ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].ase_role)
                {
                    data_path_direction = APPL_GA_ISO_OUTPUT;
                }

                APPL_TRC(
                    "BT_hci_le_remove_iso_data_path data_path_direction %d\n",
                    data_path_direction);
                retval = BT_hci_le_remove_iso_data_path
                (
                    ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_index].cis_conn_handle,
                    data_path_direction
                );

                if (API_SUCCESS != retval)
                {
                    APPL_TRC("FAILED !!! Error code = %04X\n", retval);
                }
                else
                {
                    APPL_TRC("API returned success...\n");

#ifdef APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA
                    if (APPL_GA_ISO_INPUT == data_path_direction)
                    {
                        /* Setup Module for Media Data - Generate Audio Data */
                        audio_shutdown_pl();

#ifdef INCLUDE_ETHERMIND_LC3
                        /* De-init LC3 Module */
                        LC3_encoder_delete(&lc3_enc_ctx);
                        appl_ga_lc3_enc_ready = GA_FALSE;
#endif /* INCLUDE_ETHERMIND_LC3 */
                    }
                    else if (APPL_GA_ISO_OUTPUT == data_path_direction)
                    {
                        /* Setup Module for Media Data - Play Audio Data */
                        audio_shutdown_pl();

#ifdef INCLUDE_ETHERMIND_LC3
                        /* De-init LC3 Module */
                        LC3_decoder_delete(&lc3_dec_ctx);
                        appl_ga_lc3_dec_ready = GA_FALSE;
#endif /* INCLUDE_ETHERMIND_LC3 */
                    }
#endif /* APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA */
                }
            }
        }
        break;

    case GA_SEP_RX_START_READY_NTF:
        ase = (GA_ASE_INFO*)ga_data;
        appl_ga_utils_ucc_ascs_display_common_event_info
        (
            device,
            ga_event,
            ga_status
        );
        APPL_TRC("[Event]       : GA_SEP_RX_START_READY_NTF (0x%02X)\n", ga_event);

        appl_ga_utils_ucc_ascs_display_cp_event_data(ga_status, ase);

        retval = appl_ga_fetch_rem_ase_db_index_from_ase_id(ase->data[0], &local_ase_db_index);
        if (GA_SUCCESS != retval)
        {
            printf("Unknown ASE ID\n");
        }
        else
        {
            if ((0x04 == ase->data[1])
                &&
                (APPL_GA_STATE_ENABLING == ga_rem_dev_info[curr_rem_index].ga_rem_ase[local_ase_db_index].curr_state))
            {
                ga_rem_dev_info[curr_rem_index].ga_rem_ase[local_ase_db_index].curr_state = APPL_GA_STATE_STREAMING;
                if (GA_ROLE_SOURCE == ga_rem_dev_info[curr_rem_index].ga_rem_ase[local_ase_db_index].ase_role)
                {
                    if (APPL_GA_GATT_CLIENT == curr_GATT_role)
                    {
                        /* We are Client Sink, Be ready to RX Data */
#ifdef APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA
                        audio_start_pl(AUDIO_EP_SINK);
#endif /* APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA */

                        isISODataPathReset = GA_TRUE;

#ifdef DUMP_ISO_DATA_TO_FILE
                        if (GA_TRUE == is_dump_enabled)
                        {
                            lc3_dec_media_dump_fd = fopen("lc3_dec_media_dump", "wb+");

                            if (NULL != lc3_dec_media_dump_fd)
                            {
                                APPL_TRC("Dec Media Dump created successfully\n");
                            }

                            lc3_dec_pre_media_dump_fd = fopen("lc3_dec_pre_media_dump", "wb+");

                            if (NULL != lc3_dec_pre_media_dump_fd)
                            {
                                APPL_TRC("Dec Media Dump created successfully\n");
                            }
                        }
#endif /* DUMP_ISO_DATA_TO_FILE */
                    }
                }
                else
                {
                    /* We are Client Source, Be ready to TX Data */
#ifdef APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA

                    audio_start_pl(AUDIO_EP_SOURCE);
                    ga_local_audio_src_streaming = GA_TRUE;

#ifdef DUMP_ISO_DATA_TO_FILE
                    if (GA_TRUE == is_dump_enabled)
                    {
                        lc3_enc_media_dump_fd = fopen("lc3_enc_media_dump", "wb+");
                        if (NULL != lc3_enc_media_dump_fd)
                        {
                            APPL_TRC("Enc Media Dump created successfully\n");
                        }

                        lc3_enc_pre_media_dump_fd = fopen("lc3_enc_pre_media_dump", "wb+");
                        if (NULL != lc3_enc_pre_media_dump_fd)
                        {
                            APPL_TRC("Enc Pre Media Dump created successfully\n");
                        }

                        lc3_enc_src_enqueue_fd = fopen("lc3_enc_src_enqueue_dump", "wb+");
                        if (NULL != lc3_enc_src_enqueue_fd)
                        {
                            APPL_TRC(
                                "Src Enqueue Media Dump created successfully\n");
                        }

                        lc3_audio_pl_src_callback_dump_fd = fopen("lc3_audio_pl_src_callback_dump", "wb+");
                        if (NULL != lc3_audio_pl_src_callback_dump_fd)
                        {
                            APPL_TRC(
                                "Audio Pl Src Callback Media Dump created successfully\n");
                        }
                    }
#endif /* DUMP_ISO_DATA_TO_FILE */
#endif /* APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA */
                }
            }
        }
        break;

    case GA_SEP_IDLE_NTF:
        ase = (GA_ASE_INFO*)ga_data;
        appl_ga_utils_ucc_ascs_display_common_event_info
        (
            device,
            ga_event,
            ga_status
        );
        APPL_TRC("[Event]       : GA_SEP_IDLE_NTF (0x%02X)\n", ga_event);
        appl_ga_utils_ucc_ascs_display_cp_event_data(ga_status, ase);
        break;

    default:
        printf("Received UnHandled Event\n");
        break;
    }

    GA_CB_EXIT();

    return retval;
}

GA_RESULT appl_bap_unicast_server_callback
          (
              GA_ENDPOINT * device,
              UINT8         ga_event,
              UINT16        ga_status,
              void        * ga_data,
              UINT16        ga_datalen
          )
{
    GA_RESULT     retval;

    GA_SEP_INFO * info;
    GA_SEP_CONF * sep;
    GA_METADATA * meta;
    GA_QOS_CONF * qos_conf;
    GA_LC3_CODEC_SPECIFIC_CONF cs_conf_ind;
    UINT8                      data_path_direction;
    UINT8 local_ase_db_index;
    UINT16 audio_ctx;
    UINT16 response[2] = { 0 };

    if (NULL == device)
    {
        printf("NULL Device Unexpected!\n");
        return GA_FAILURE;
    }

    GA_CB_ENTRY("Server");

    GA_IGNORE_UNUSED_PARAM(ga_status);
    GA_IGNORE_UNUSED_PARAM(ga_datalen);

    /* Init */
    retval = GA_SUCCESS;
    data_path_direction = APPL_GA_ISO_UNKNOWN;
    local_ase_db_index = 0;

    switch (ga_event)
    {
    case GA_SEP_CONFIGURE_IND:
        appl_ga_utils_ucs_ascs_display_common_event_info
        (
            device,
            ga_status
        );
        APPL_TRC
        (
            "[Event]       : GA_SEP_CONFIGURE_IND (0x%02X)\n",
            ga_event
        );
        APPL_TRC("\n");
        appl_ga_utils_ascs_display_char_uuid(GA_CHAR_ASCS_ASE_CP);
        info = (GA_SEP_INFO*)ga_data;
        sep = (GA_SEP_CONF*)info->info;
        APPL_TRC("RECEIVED DATA\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%02X\n", ga_datalen);
        APPL_TRC("Data:\n");
        APPL_TRC
        (
            "Len: 0x%02X,  ASE_ID: %d\n",
            (UCHAR)sizeof(info->sep_id), info->sep_id
        );
        APPL_TRC
        (
            "Len: 0x%02X,  ",
            (UCHAR)sizeof(sep->role)
        );
        appl_ga_utils_bap_display_role(sep->role);
        APPL_TRC
        (
            "Len: 0x%02X,  Target_Latency: 0x%02X\n",
            (UCHAR)sizeof(sep->target_latency), sep->target_latency
        );
        APPL_TRC
        (
            "Len: 0x%02X,  Target_PHY:",
            (UCHAR)sizeof(sep->target_phy)
        );
        appl_ga_utils_bap_display_pref_phy(sep->target_phy);
        appl_ga_utils_bap_display_codec_id(sep->codec);
        APPL_TRC
        (
            "Len: 0x%02X,  Codec_Specific_Configuration_Length: %d\n",
            (UCHAR)sizeof(sep->role), sep->codec.ie_len
        );
        APPL_TRC
        (
            "Len: 0x%02X,  Codec_Specific_Configuration:\n",
            (UCHAR)sizeof(sep->codec.ie)
        );
        /* TODO: Check. As coding format is giving zero, should we dump the data or parse it ? */
        appl_ga_utils_bap_parse_and_display_codec_spec_conf_ltv
        (
            sep->codec.ie,
            sep->codec.ie_len,
            &cs_conf_ind
        );

        if (GA_FAILURE == appl_ga_fetch_local_ase_db_index_from_ase_index(info->sep_id, &local_ase_db_index))
        {
            printf("ASE ID not found\n");
        }
        else
        {
            ga_local_audio_ase[local_ase_db_index].curr_state = APPL_GA_STATE_CODEC_CONF;
        }

        break;

    case GA_SEP_SETUP_IND:
        appl_ga_utils_ucs_ascs_display_common_event_info
        (
            device,
            ga_status
        );
        APPL_TRC
        (
            "[Event]       : GA_SEP_SETUP_IND (0x%02X)\n",
            ga_event
        );
        APPL_TRC("\n");
        appl_ga_utils_ascs_display_char_uuid(GA_CHAR_ASCS_ASE_CP);

        info = (GA_SEP_INFO*)ga_data;
        qos_conf = (GA_QOS_CONF*)info->info;
        APPL_TRC("RECEIVED DATA\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%02X\n", ga_datalen);
        APPL_TRC("Data:\n");
        APPL_TRC
        (
            "Len: 0x%02X,  ASE_ID: %d\n",
            (UCHAR)sizeof(info->sep_id), info->sep_id
        );
        APPL_TRC
        (
            "Len: 0x%02X,  CIG_ID - 0x%02X\n",
            (UCHAR)sizeof(qos_conf->channel.cig_id),
            qos_conf->channel.cig_id
        );
        APPL_TRC
        (
            "Len: 0x%02X,  CIS_ID - 0x%02X\n",
            (UCHAR)sizeof(qos_conf->channel.cis_id),
            qos_conf->channel.cis_id
        );
        APPL_TRC
        (
            "Len: 0x%02X,  SDU_Interval: %d(0x%08X)\n",
            (UCHAR)sizeof(qos_conf->sdu_interval),
            qos_conf->sdu_interval,
            qos_conf->sdu_interval
        );
        APPL_TRC
        (
            "Len: 0x%02X,  Framing: ",
            (UCHAR)sizeof(qos_conf->framing)
        );
        appl_ga_utils_bap_display_framing(qos_conf->framing);
        APPL_TRC
        (
            "Len: 0x%02X,  PHY:",
            (UCHAR)sizeof(qos_conf->phy)
        );
        appl_ga_utils_bap_display_pref_phy(qos_conf->phy);
        APPL_TRC
        (
            "Len: 0x%02X,  Max_SDU: %d(0x%04X)\n",
            (UCHAR)sizeof(qos_conf->max_sdu),
            qos_conf->max_sdu,
            qos_conf->max_sdu
        );
        APPL_TRC
        (
            "Len: 0x%02X,  Retransmission_Number: (%d)0x%02X\n",
            (UCHAR)sizeof(qos_conf->retxn),
            qos_conf->retxn,
            qos_conf->retxn
        );
        APPL_TRC
        (
            "Len: 0x%02X,  Max_Transport_Latency: %d(0x%04X)\n",
            (UCHAR)sizeof(qos_conf->max_transport_latency),
            qos_conf->max_transport_latency,
            qos_conf->max_transport_latency
        );
        APPL_TRC
        (
            "Len: 0x%02X,  Presentation_Delay: %d(0x%08X)\n",
            (UCHAR)sizeof(qos_conf->presentation_delay),
            qos_conf->presentation_delay,
            qos_conf->presentation_delay
        );

        if (GA_FAILURE == appl_ga_fetch_local_ase_db_index_from_ase_index(info->sep_id, &local_ase_db_index))
        {
            printf("ASE ID not found\n");
        }
        else
        {
            GA_mem_copy(&ga_local_audio_ase[local_ase_db_index].qos_conf, qos_conf, sizeof(GA_QOS_CONF));
            ga_local_audio_ase[local_ase_db_index].curr_state = APPL_GA_STATE_QOS_CONF;

            APPL_TRC("ASE Index: %d, ASE State: APPL_GA_STATE_QOS_CONF\n", local_ase_db_index);
        }
        break;

    case GA_SEP_START_IND:
        appl_ga_utils_ucs_ascs_display_common_event_info
        (
            device,
            ga_status
        );
        APPL_TRC
        (
            "[Event]       : GA_SEP_START_IND (0x%02X)\n",
            ga_event
        );
        APPL_TRC("\n");
        appl_ga_utils_ascs_display_char_uuid(GA_CHAR_ASCS_ASE_CP);

        info = (GA_SEP_INFO*)ga_data;
        meta = (GA_METADATA*)info->info;

        APPL_TRC("RECEIVED DATA\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%02X\n", ga_datalen);
        APPL_TRC("Data:\n");

        APPL_TRC
        (
            "Len: 0x%02X,  ASE_ID: %d\n",
            (UCHAR)sizeof(info->sep_id), info->sep_id
        );
        APPL_TRC
        (
            "Len: 0x%02X,  Metadata_Length - 0x%02X\n",
            (UCHAR)sizeof(meta->length),
            meta->length
        );
        APPL_TRC("Len: 0x%02X,  Metadata -\n", meta->length);
        appl_ga_utils_bap_parse_and_display_metadata_ltv
        (
            meta->data,
            meta->length,
            &audio_ctx,
            NULL
        );

        audio_ctx = 0xFFFF;
        if (GA_FAILURE == appl_ga_fetch_local_ase_db_index_from_ase_index(info->sep_id, &local_ase_db_index))
        {
            printf("ASE ID not found\n");
        }
        else
        {
            /* Do not accept the Metadata Supp Audio Context Type if not avail */
            if (GA_ROLE_SINK == ga_local_audio_ase[local_ase_db_index].codec_conf.role)
            {
                if ((0xFFFF != audio_ctx) && (!(audio_ctx & ga_local_pacs.avail_sink_contexts)))
                {
                    response[0] = 0x0B; /* Response Code */
                    response[1] = GA_LTV_METADATA_STREAMING_AC_TYPE; /* Reason */
                    GA_UNPACK_LE_2_BYTE(&retval, response);
                }
                else
                {
                    ga_local_audio_ase[local_ase_db_index].curr_state = APPL_GA_STATE_ENABLING;
                }
            }
            else if (GA_ROLE_SOURCE == ga_local_audio_ase[local_ase_db_index].codec_conf.role)
            {
                if ((0xFFFF != audio_ctx) && (!(audio_ctx & ga_local_pacs.avail_src_contexts)))
                {
                    response[0] = 0x0B; /* Response Code */
                    response[1] = GA_LTV_METADATA_STREAMING_AC_TYPE; /* Reason */
                    GA_UNPACK_LE_2_BYTE(&retval, response);
                }
                else
                {
                    ga_local_audio_ase[local_ase_db_index].curr_state = APPL_GA_STATE_ENABLING;
                }
            }
        }
        break;

    case GA_SEP_SUSPEND_IND:
        appl_ga_utils_ucs_ascs_display_common_event_info
        (
            device,
            ga_status
        );
        APPL_TRC
        (
            "[Event]       : GA_SEP_SUSPEND_IND (0x%02X)\n",
            ga_event
        );
        APPL_TRC("\n");
        appl_ga_utils_ascs_display_char_uuid(GA_CHAR_ASCS_ASE_CP);

        info = (GA_SEP_INFO*)ga_data;

        APPL_TRC("RECEIVED DATA\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%02X\n", ga_datalen);
        APPL_TRC("Data:\n");

        APPL_TRC
        (
            "Len: 0x%02X,  ASE_ID: %d\n",
            (UCHAR)sizeof(info->sep_id), info->sep_id
        );

        if (GA_FAILURE == appl_ga_fetch_local_ase_db_index_from_ase_index(info->sep_id, &local_ase_db_index))
        {
            printf("Unknown ASE ID\n");
        }
        else
        {
            ga_local_audio_ase[local_ase_db_index].curr_state = APPL_GA_STATE_DISABLING;
#ifdef APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA
            if (GA_ROLE_SINK == ga_local_audio_ase[local_ase_db_index].codec_conf.role)
            {
                audio_stop_pl(AUDIO_EP_SINK);
                ga_local_audio_ase[local_ase_db_index].curr_state = APPL_GA_STATE_SUSPENDED;
                BT_debug_trace(
                BT_MODULE_ID_WT, "async APPL_GA_STATE_SUSPENDED 0x%02x\n",
                local_ase_db_index);
            }
            else if (GA_ROLE_SOURCE == ga_local_audio_ase[local_ase_db_index].codec_conf.role)
            {
                audio_stop_pl(AUDIO_EP_SOURCE);
                BT_debug_trace(
                BT_MODULE_ID_WT, "async APPL_GA_STATE_SUSPENDED 0x%02x\n",
                local_ase_db_index);
            }
#endif /* APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA */
        }
        break;

    case GA_SEP_RX_START_READY_IND:
        appl_ga_utils_ucs_ascs_display_common_event_info
        (
            device,
            ga_status
        );
        APPL_TRC
        (
            "[Event]       : GA_SEP_RX_START_READY_IND (0x%02X)\n",
            ga_event
        );
        APPL_TRC("\n");
        appl_ga_utils_ascs_display_char_uuid(GA_CHAR_ASCS_ASE_CP);

        info = (GA_SEP_INFO*)ga_data;

        APPL_TRC("RECEIVED DATA\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%02X\n", ga_datalen);
        APPL_TRC("Data:\n");

        APPL_TRC
        (
            "Len: 0x%02X,  ASE_ID: %d\n",
            (UCHAR)sizeof(info->sep_id), info->sep_id
        );

        if (APPL_GA_GATT_SERVER == curr_GATT_role)
        {
            retval = appl_ga_fetch_local_ase_db_index_from_ase_index
                     (
                         info->sep_id,
                         &local_ase_db_index
                     );
            if (GA_SUCCESS != retval)
            {
                printf("Unknown ASE ID\n");
            }
            else
            {
                if ((APPL_GA_STATE_ENABLING == ga_local_audio_ase[local_ase_db_index].curr_state)
                    &&
                    (GA_ROLE_SOURCE == ga_local_audio_ase[local_ase_db_index].codec_conf.role))
                {
                    ga_local_audio_ase[local_ase_db_index].curr_state = APPL_GA_STATE_STREAMING;
                    BT_debug_trace(
                    BT_MODULE_ID_WT, "async Server APPL_GA_STATE_STREAMING 0x%02x\n",
                    local_ase_db_index);
                }
            }
        }
        if (GA_SUCCESS == retval)
        {
#ifdef APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA

            audio_start_pl(AUDIO_EP_SOURCE);
            ga_local_audio_src_streaming = GA_TRUE;

            BT_debug_trace(
            BT_MODULE_ID_WT, "async Src ga_local_audio_src_streaming 0x%02x\n",
            local_ase_db_index);

#ifdef DUMP_ISO_DATA_TO_FILE
            if (GA_TRUE == is_dump_enabled)
            {
                lc3_enc_media_dump_fd = fopen("lc3_enc_media_dump", "wb+");
                if (NULL != lc3_enc_media_dump_fd)
                {
                    APPL_TRC("Enc Media Dump created successfully\n");
                }

                lc3_enc_pre_media_dump_fd = fopen("lc3_enc_pre_media_dump", "wb+");
                if (NULL != lc3_enc_pre_media_dump_fd)
                {
                    APPL_TRC("Pre Enc Media Dump created successfully\n");
                }

                lc3_enc_src_enqueue_fd = fopen("lc3_enc_src_enqueue_dump", "wb+");
                if (NULL != lc3_enc_src_enqueue_fd)
                {
                    APPL_TRC("Src Enqueue Media Dump created successfully\n");
                }

                lc3_audio_pl_src_callback_dump_fd = fopen("lc3_audio_pl_src_callback_dump", "wb+");
                if (NULL != lc3_audio_pl_src_callback_dump_fd)
                {
                    APPL_TRC(
                        "Audio Pl Src Callback Media Dump created successfully\n");
                }
            }
#endif /* DUMP_ISO_DATA_TO_FILE */
#endif /* APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA */
        }
        break;

    case GA_SEP_RX_STOP_READY_IND:
        appl_ga_utils_ucs_ascs_display_common_event_info
        (
            device,
            ga_status
        );
        APPL_TRC
        (
            "[Event]       : GA_SEP_RX_STOP_READY_IND (0x%02X)\n",
            ga_event
        );
        APPL_TRC("\n");
        appl_ga_utils_ascs_display_char_uuid(GA_CHAR_ASCS_ASE_CP);

        info = (GA_SEP_INFO*)ga_data;

        APPL_TRC("RECEIVED DATA\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%02X\n", ga_datalen);
        APPL_TRC("Data:\n");

        APPL_TRC
        (
            "Len: 0x%02X,  ASE_ID: %d\n",
            (UCHAR)sizeof(info->sep_id), info->sep_id
        );

        if (GA_FAILURE == appl_ga_fetch_local_ase_db_index_from_ase_index(info->sep_id, &local_ase_db_index))
        {
            printf("Unknown ASE ID\n");
        }
        else
        {
            /* Ideally this ASE will be a Source ASE */
            ga_local_audio_ase[local_ase_db_index].curr_state = APPL_GA_STATE_SUSPENDED;
            ga_local_audio_src_streaming = GA_FALSE;
        }

        break;

    case GA_SEP_UPDATE_IND:
        appl_ga_utils_ucs_ascs_display_common_event_info
        (
            device,
            ga_status
        );
        APPL_TRC
        (
            "[Event]       : GA_SEP_UPDATE_IND (0x%02X)\n",
            ga_event
        );
        APPL_TRC("\n");
        appl_ga_utils_ascs_display_char_uuid(GA_CHAR_ASCS_ASE_CP);

        info = (GA_SEP_INFO *)ga_data;
        meta = (GA_METADATA *)info->info;

        APPL_TRC("RECEIVED DATA\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%02X\n", ga_datalen);
        APPL_TRC("Data:\n");
        APPL_TRC
        (
            "Len: 0x%02X,  ASE_ID: %d\n",
            (UCHAR)sizeof(info->sep_id), info->sep_id
        );
        APPL_TRC
        (
            "Len: 0x%02X,  Metadata_Length - 0x%02X\n",
            (UCHAR)sizeof(meta->length),
            meta->length
        );
        APPL_TRC("Len: 0x%02X,  Metadata -\n", meta->length);

        audio_ctx = 0xFFFF;

        if (GA_FAILURE == appl_ga_fetch_local_ase_db_index_from_ase_index(info->sep_id, &local_ase_db_index))
        {
            printf("Unknown ASE ID\n");
        }
        else
        {
            /* Parse and display the Metadata_LTV. */
            appl_ga_utils_bap_parse_and_display_metadata_ltv
            (
                meta->data,
                meta->length,
                &audio_ctx,
                NULL
            );
            /* Do not accept the Metadata Supp Audio Context Type if not avail */
            if  (GA_ROLE_SINK == ga_local_audio_ase[local_ase_db_index].codec_conf.role)
            {
                if ((0xFFFF != audio_ctx) && (!(audio_ctx & ga_local_pacs.avail_sink_contexts)))
                {
                    response[0] = 0x0B; /* Response Code */
                    response[1] = GA_LTV_METADATA_STREAMING_AC_TYPE; /* Reason */
                    GA_UNPACK_LE_2_BYTE(&retval, response);
                }
            }
            else if (GA_ROLE_SOURCE == ga_local_audio_ase[local_ase_db_index].codec_conf.role)
            {
                if ((0xFFFF != audio_ctx) && (!(audio_ctx & ga_local_pacs.avail_src_contexts)))
                {
                    response[0] = 0x0B; /* Response Code */
                    response[1] = GA_LTV_METADATA_STREAMING_AC_TYPE; /* Reason */
                    GA_UNPACK_LE_2_BYTE(&retval, response);
                }
            }
        }

        break;

    case GA_SEP_RELEASE_IND:
        appl_ga_utils_ucs_ascs_display_common_event_info
        (
            device,
            ga_status
        );
        APPL_TRC
        (
            "[Event]       : GA_SEP_RELEASE_IND (0x%02X)\n",
            ga_event
        );
        APPL_TRC("\n");
        appl_ga_utils_ascs_display_char_uuid(GA_CHAR_ASCS_ASE_CP);

        info = (GA_SEP_INFO *)ga_data;

        APPL_TRC("RECEIVED DATA\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%02X\n", ga_datalen);
        APPL_TRC("Data:\n");

        APPL_TRC
        (
            "Len: 0x%02X,  ASE_ID: %d\n",
            (UCHAR)sizeof(info->sep_id), info->sep_id
        );

        if (GA_FAILURE == appl_ga_fetch_local_ase_db_index_from_ase_index(info->sep_id, &local_ase_db_index))
        {
            printf("Unknown ASE ID\n");
        }
        else
        {
            ga_local_audio_ase[local_ase_db_index].curr_state = APPL_GA_STATE_DISABLING;

            if (GA_ROLE_SOURCE == ga_local_audio_ase[local_ase_db_index].codec_conf.role)
            {
                data_path_direction = APPL_GA_ISO_INPUT;
#ifdef APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA
                audio_stop_pl(AUDIO_EP_SOURCE);
#endif /* APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA */
            }
            else if (GA_ROLE_SINK == ga_local_audio_ase[local_ase_db_index].codec_conf.role)
            {
                data_path_direction = APPL_GA_ISO_OUTPUT;
#ifdef APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA
                audio_stop_pl(AUDIO_EP_SINK);
#endif /* APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA */
            }

            APPL_TRC(
            "BT_hci_le_remove_iso_data_path data_path_direction %d\n",
            data_path_direction);
            retval = BT_hci_le_remove_iso_data_path
                     (
                         ga_local_audio_ase[local_ase_db_index].cis_conn_handle,
                         data_path_direction
                     );

            if (API_SUCCESS != retval)
            {
                APPL_TRC("FAILED !!! Error code = %04X\n", retval);
            }
            else
            {
                APPL_TRC("API returned success...\n");

                ga_local_audio_ase[local_ase_db_index].curr_state = APPL_GA_STATE_RELEASING;

#ifdef APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA
                if (APPL_GA_ISO_INPUT == data_path_direction)
                {
                    /* Setup Module for Media Data - Generate Audio Data */
                    audio_shutdown_pl();

#ifdef INCLUDE_ETHERMIND_LC3
                    /* De-init LC3 Module */
                    LC3_encoder_delete(&lc3_enc_ctx);
                    appl_ga_lc3_enc_ready = GA_FALSE;
#endif /* INCLUDE_ETHERMIND_LC3 */
                }
                else if (APPL_GA_ISO_OUTPUT == data_path_direction)
                {
                    /* Setup Module for Media Data - Play Audio Data */
                    audio_shutdown_pl();

#ifdef INCLUDE_ETHERMIND_LC3
                    /* De-init LC3 Module */
                    LC3_decoder_delete(&lc3_dec_ctx);
                    appl_ga_lc3_dec_ready = GA_FALSE;
#endif /* INCLUDE_ETHERMIND_LC3 */
                }
#endif /* APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA */
            }
        }
        break;

    default:
        printf("Received UnHandled Event\n");
        break;
    }

    GA_CB_EXIT();

    return retval;
}

#ifdef BAP_BROADCAST
GA_RESULT appl_bap_broadcast_source_callback
          (
              GA_ENDPOINT  * device,
              UINT8    ga_event,
              UINT16   ga_status,
              void   * ga_data,
              UINT16   ga_datalen
          )
{
    GA_RESULT  retval;

    UINT8 i;
    BAP_BC_START_PARAMS bc_start = { 0 };
    BAP_BC_SUSPEND_PARAMS bc_suspend = { 0 };

    /* Initialize */
    retval = GA_SUCCESS;
    i = 0U;

    APPL_DBG("[APPL][GA][BC_SRC]: >> appl_ga_bc_src_handler\n");

    switch (ga_event)
    {
    case GA_BC_SETUP_ANNOUNCEMENT_CNF:
        GA_CB_ENTRY("BC Source");
        appl_ga_utils_bc_display_src_console_header(device, ga_status);
        APPL_TRC
        (
            "[Event]       : GA_BC_SETUP_ANNOUNCEMENT_CNF (0x%02X)\n",
            ga_event
        );
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%02X\n", ga_datalen);
        if (0U == ga_datalen)
        {
            APPL_TRC("Data: NULL\n");
        }
        break;

    case GA_BC_END_ANNOUNCEMENT_CNF:
        GA_CB_ENTRY("BC Source");
        appl_ga_utils_bc_display_src_console_header(device, ga_status);
        APPL_TRC
        (
            "[Event]       : GA_BC_END_ANNOUNCEMENT_CNF (0x%02X)\n",
            ga_event
        );
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%02X\n", ga_datalen);
        if (0U == ga_datalen)
        {
            APPL_TRC("Data: NULL\n");
        }
        break;

    case GA_BC_START_CNF:
        GA_CB_ENTRY("BC Source");
        appl_ga_utils_bc_display_src_console_header(device, ga_status);
        APPL_TRC
        (
            "[Event]       : GA_BC_START_CNF (0x%02X)\n",
            ga_event
        );
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();

        APPL_TRC("Data Length: 0x%02X\n", ga_datalen);
        APPL_TRC("Data:\n");
        if (GA_SUCCESS ==
            GA_broadcast_decode_event_params(GA_BC_START_CNF, ga_data, &bc_start))
        {
            APPL_TRC
            (
                "Len: 0x%02X,  Error Code: 0x%02X\n",
                (UCHAR)sizeof(bc_start.error_code),
                bc_start.error_code
            );
            APPL_TRC
            (
                "Len: 0x%02X,  BIG Handle: 0x%02X\n",
                (UCHAR)sizeof(bc_start.big_handle),
                bc_start.big_handle
            );
            APPL_TRC
            (
                "Len: 0x%02X,  BIG Sync Delay: 0x%08X\n",
                (UCHAR)sizeof(bc_start.big_sync_delay),
                bc_start.big_sync_delay
            );
            APPL_TRC
            (
                "Len: 0x%02X,  Transport Latency BIG: 0x%08X\n",
                (UCHAR)sizeof(bc_start.transport_latency_big),
                bc_start.transport_latency_big
            );
            APPL_TRC
            (
                "Len: 0x%02X,  Phy: 0x%02X\n",
                (UCHAR)sizeof(bc_start.phy), bc_start.phy
            );
            APPL_TRC
            (
                "Len: 0x%02X,  NSE: 0x%02X\n",
                (UCHAR)sizeof(bc_start.nse), bc_start.nse
            );
            APPL_TRC
            (
                "Len: 0x%02X,  BN: 0x%02X\n",
                (UCHAR)sizeof(bc_start.bn), bc_start.bn
            );
            APPL_TRC
            (
                "Len: 0x%02X,  PTO: 0x%02X\n",
                (UCHAR)sizeof(bc_start.pto), bc_start.pto
            );
            APPL_TRC
            (
                "Len: 0x%02X,  IRC: 0x%02X\n",
                (UCHAR)sizeof(bc_start.irc), bc_start.irc
            );
            APPL_TRC
            (
                "Len: 0x%02X,  Max PDU: 0x%04X\n",
                (UCHAR)sizeof(bc_start.max_pdu), bc_start.max_pdu
            );
            APPL_TRC
            (
                "Len: 0x%02X,  ISO Interval: 0x%04X\n",
                (UCHAR)sizeof(bc_start.iso_interval), bc_start.iso_interval
            );

            APPL_TRC("Number of BISes 0x%02X\n", bc_start.num_bis);

            for (i = 0; i < bc_start.num_bis; i++)
            {
                APPL_TRC("BIS Conn Handle 0x%04X\n", bc_start.connection_handle[i]);
            }

            if (0U < bc_start.num_bis)
            {
#ifdef APPL_END_ANNOUNCEMENT_ON_STREAMING
                /* Broadcast release: BIG is created, Now disable Advertising */
                APPL_TRC("GA_broadcast_end_announcement\n");
                retval = GA_broadcast_end_announcement
                (
                    appl_bcast_ssn
                );
                APPL_TRC("Retval - ");
                appl_ga_utils_display_ga_result(retval);
    #endif /* APPL_END_ANNOUNCEMENT_ON_STREAMING */

                appl_ga_prep_bcast_for_streaming(0x01, bc_start.connection_handle[0]);

                /* We are Source, Be ready to TX Data */
    #ifdef APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA

                audio_start_pl(AUDIO_EP_SOURCE);
                ga_local_audio_src_streaming = GA_TRUE;

    #ifdef DUMP_ISO_DATA_TO_FILE
                lc3_enc_media_dump_fd = fopen("lc3_enc_media_dump", "wb+");
                if (NULL != lc3_enc_media_dump_fd)
                {
                    APPL_TRC("Enc Media Dump created successfully\n");
                }

                lc3_enc_pre_media_dump_fd = fopen("lc3_enc_pre_media_dump", "wb+");
                if (NULL != lc3_enc_pre_media_dump_fd)
                {
                    APPL_TRC("Enc Pre Media Dump created successfully\n");
                }

                lc3_enc_src_enqueue_fd = fopen("lc3_enc_src_enqueue_dump", "wb+");
                if (NULL != lc3_enc_src_enqueue_fd)
                {
                    APPL_TRC("Src Enqueue Media Dump created successfully\n");
                }

                lc3_audio_pl_src_callback_dump_fd = fopen("lc3_audio_pl_src_callback_dump", "wb+");
                if (NULL != lc3_audio_pl_src_callback_dump_fd)
                {
                    APPL_TRC("Audio Pl Src Callback Media Dump created successfully\n");
                }
    #endif /* DUMP_ISO_DATA_TO_FILE */
    #endif /* APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA */
            }
        }
        break;

    case GA_BC_SUSPEND_CNF:
        GA_CB_ENTRY("BC Source");
        appl_ga_utils_bc_display_src_console_header(device, ga_status);
        APPL_TRC
        (
            "[Event]       : GA_BC_SUSPEND_CNF (0x%02X)\n",
            ga_event
        );
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();

        APPL_TRC("Data Length: 0x%02X\n", ga_datalen);
        APPL_TRC("Data:\n");
        if (GA_SUCCESS ==
            GA_broadcast_decode_event_params(GA_BC_SUSPEND_CNF, ga_data, &bc_suspend))
        {
            APPL_TRC
            (
                "Len: 0x%02X,  BIG Handle: 0x%02X\n",
                (UCHAR)sizeof(bc_suspend.big_handle), bc_suspend.big_handle
            );
            APPL_TRC
            (
                "Len: 0x%02X,  Reason: 0x%02X\n",
                (UCHAR)sizeof(bc_suspend.reason), bc_suspend.reason
            );
        }
        break;

    case GA_BC_SUSPEND_IND:
        GA_CB_ENTRY("BC Source");
        appl_ga_utils_bc_display_src_console_header(device, ga_status);
        APPL_TRC
        (
            "[Event]       : GA_BC_SUSPEND_IND (0x%02X)\n",
            ga_event
        );
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();

        APPL_TRC("Data Length: 0x%02X\n", ga_datalen);
        APPL_TRC("Data:\n");
        if (GA_SUCCESS ==
            GA_broadcast_decode_event_params(GA_BC_SUSPEND_IND, ga_data, &bc_suspend))
        {
            APPL_TRC
            (
                "Len: 0x%02X,  BIG Handle: 0x%02X\n",
                (UCHAR)sizeof(bc_suspend.big_handle), bc_suspend.big_handle
            );
            APPL_TRC
            (
                "Len: 0x%02X,  Reason: 0x%02X\n",
                (UCHAR)sizeof(bc_suspend.reason), bc_suspend.reason
            );
        }
        break;

    default:
        APPL_ERR("Received UnHandled Event !\n");
        break;
    }

    GA_CB_EXIT();

    APPL_DBG("[APPL][GA][BC_SRC]: << appl_ga_bc_src_handler\n");
    return retval;
}

GA_RESULT appl_bap_broadcast_sink_callback
          (
              GA_ENDPOINT   * ga_device,
              UINT8    ga_event,
              UINT16   ga_status,
              void   * ga_data,
              UINT16   ga_datalen
          )
{
    GA_RESULT  retval;
    UINT8      i;
    UINT32     delay;
    UINT8      n_sg, sg_index;
    UINT8      n_bis;
    UINT8      num_bis;
    UINT8      parsed_len;

    BAP_BC_SOURCE_ANNOUNCEMENT_PARAMS bc_source_announcement;
    BAP_BROADCAST_ANNOUNCEMENT bc_annoucement = { 0 };
    BAP_BASIC_ANNOUNCEMENT_LEVEL_1 bc_basic_announcement_1 = { 0 };
    BAP_BASIC_ANNOUNCEMENT_LEVEL_2 bc_basic_announcement_2 = { 0 };
    BAP_BC_SOURCE_CONFIG_PARAMS bc_source_config;
    BAP_BC_SOURCE_STREAM_PARAMS bc_source_stream;
    BAP_BC_ASSOCIATE_PARAMS bc_associate;
    BAP_BC_DISSOCIATE_PARAMS bc_dissociate;
    BAP_BC_ENABLE_PARAMS bc_enable;
    BAP_BC_DISABLE_PARAMS bc_disable;

    GA_LC3_CODEC_SPECIFIC_CONF rem_cs_conf = { 0 };

    /* Initialize */
    retval = GA_SUCCESS;
    delay = 0U;
    n_sg = 0U;
    sg_index = 0U;
    n_bis = 0U;
    num_bis = 0U;
    parsed_len = 0U;

    /* GA_CB_ENTRY("BCast Sink"); */

    switch (ga_event)
    {
    case GA_BC_SCAN_ANNOUNCEMENT_CNF:
        GA_CB_ENTRY("BC CAP Server");
        appl_ga_utils_bc_display_sink_console_header(ga_device, ga_status);
        APPL_TRC
        (
            "[Event]       : GA_BC_SCAN_ANNOUNCEMENT_CNF (0x%02X)\n",
            ga_event
        );
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%02X\n", ga_datalen);
        if (0U == ga_datalen)
        {
            APPL_TRC("Data: NULL\n");
        }
        break;

    case GA_BC_SOURCE_ANNOUNCEMENT_IND:
    {
        if (GA_SUCCESS ==
            GA_broadcast_decode_event_params
            (
                GA_BC_SOURCE_ANNOUNCEMENT_IND,
                ga_data,
                &bc_source_announcement
            ))
        {
            if (GA_SUCCESS == GA_broadcast_decode_broadcast_announcement
                                (
                                    bc_source_announcement.data,
                                    bc_source_announcement.datalen,
                                    &bc_annoucement
                                ))
            {
                if (found_bc_src == GA_FALSE)
                {
                    if (GA_BRR_DEV_ADDR_IS_NON_ZERO(bc_src_interested.bd_addr))
                    {
                        if (GA_COMPARE_ENDPOINT_ADDR
                           (
                               &bc_src_interested,
                               bc_source_announcement.address
                           ))
                        {
                            found_bc_src = GA_TRUE;
                        }
                    }
                    else
                    {
                        found_bc_src = GA_TRUE;
                    }

                    /* Display */
                    GA_CB_ENTRY("BC CAP Server");
                    appl_ga_utils_bc_display_sink_console_header(ga_device, ga_status);
                    APPL_TRC
                    (
                        "[Event]       : GA_BC_SOURCE_ANNOUNCEMENT_IND (0x%02X)\n",
                        ga_event
                    );
                    APPL_TRC("\n");
                    GA_FORMAT_DISPLAY();

                    APPL_TRC
                    (
                        "Len: 0x%02X,  Event Type: 0x%04X\n",
                        (UCHAR)sizeof(UINT16), bc_source_announcement.event_type
                    );
                    APPL_TRC
                    (
                        "Len: 0x%02X,  Address Type: 0x%02X\n",
                        (UCHAR)sizeof(UINT8), bc_source_announcement.address_type
                    );
                    APPL_TRC
                    (
                        "Len: 0x%02X,  Address: %02X:%02X:%02X:%02X:%02X:%02X\n",
                        GA_BD_ADDR_SIZE,
                        bc_source_announcement.address[0],
                        bc_source_announcement.address[1],
                        bc_source_announcement.address[2],
                        bc_source_announcement.address[3],
                        bc_source_announcement.address[4],
                        bc_source_announcement.address[5]
                    );
                    APPL_TRC
                    (
                        "Len: 0x%02X,  Primary Phy: 0x%02X\n",
                        (UCHAR)sizeof(UINT8), bc_source_announcement.primary_phy
                    );
                    APPL_TRC
                    (
                        "Len: 0x%02X,  Secondary Phy: 0x%02X\n",
                        (UCHAR)sizeof(UINT8), bc_source_announcement.secondary_phy
                    );
                    APPL_TRC
                    (
                        "Len: 0x%02X,  SID: 0x%02X\n",
                        (UCHAR)sizeof(UINT8), bc_source_announcement.advertising_sid
                    );
                    APPL_TRC
                    (
                        "Len: 0x%02X,  Tx Power: %d\n",
                        (UCHAR)sizeof(INT8), bc_source_announcement.tx_power
                    );
                    APPL_TRC
                    (
                        "Len: 0x%02X,  RSSI: %d\n",
                        (UCHAR)sizeof(INT8), bc_source_announcement.rssi
                    );
                    APPL_TRC
                    (
                        "Len: 0x%02X,  PA Interval: 0x%04X\n",
                        (UCHAR)sizeof(UINT16),
                        bc_source_announcement.periodic_adv_interval
                    );
                    APPL_TRC
                    (
                        "Len: 0x%02X,  Direct Address Type: 0x%02X\n",
                        (UCHAR)sizeof(UINT8),
                        bc_source_announcement.direct_address_type
                    );
                    APPL_TRC
                    (
                        "Len: 0x%02X,  Direct Address: %02X:%02X:%02X:%02X:%02X:%02X\n",
                        GA_BD_ADDR_SIZE,
                        bc_source_announcement.direct_address[0],
                        bc_source_announcement.direct_address[1],
                        bc_source_announcement.direct_address[2],
                        bc_source_announcement.direct_address[3],
                        bc_source_announcement.direct_address[4],
                        bc_source_announcement.direct_address[5]
                    );
                    APPL_TRC("Data Length: 0x%02X\n", bc_source_announcement.datalen);

                    APPL_TRC("Data:\n");
                    APPL_TRC
                    (
                        "Len: 0x%02X,  Broadcast_ID: 0x%06X\n",
                        BAP_BC_ID_LEN, bc_annoucement.broadcast_id
                    );
                }
            }
        }
    }
    break;

    case GA_BC_SCAN_END_CNF:
        GA_CB_ENTRY("BC CAP Server");
        appl_ga_utils_bc_display_sink_console_header(ga_device, ga_status);
        APPL_TRC
        (
            "[Event]       : GA_BC_SCAN_END_CNF (0x%02X)\n",
            ga_event
        );
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%02X\n", ga_datalen);
        if (0U == ga_datalen)
        {
            APPL_TRC("Data: NULL\n");
        }

        if (GA_SUCCESS == ga_status)
        {
            found_bc_src = GA_FALSE;
        }
        break;

    case GA_BC_SOURCE_CONFIG_IND:
        if (GA_SUCCESS ==
            GA_broadcast_decode_event_params(GA_BC_SOURCE_CONFIG_IND, ga_data, &bc_source_config))
        {
            if (appl_bcast_config_synchandle == appl_bcast_synchandle)
            {
                /* Skip printing of this received event multiple times */
                break;
            }

            appl_bcast_config_synchandle = appl_bcast_synchandle;

            GA_CB_ENTRY("BC CAP Server");
            appl_ga_utils_bc_display_sink_console_header(ga_device, ga_status);
            APPL_TRC
            (
                "[Event]       : GA_BC_SOURCE_CONFIG_IND (0x%02X)\n",
                ga_event
            );
            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();

            APPL_TRC
            (
                "Len: 0x%02X,  Sync Handle: 0x%04X\n",
                (UCHAR)sizeof(UINT16), bc_source_config.sync_handle
            );
            APPL_TRC
            (
                "Len: 0x%02X,  Tx Power: %d\n",
                (UCHAR)sizeof(INT8), bc_source_config.tx_power
            );
            APPL_TRC
            (
                "Len: 0x%02X,  RSSI: %d\n",
                (UCHAR)sizeof(INT8), bc_source_config.rssi
            );
            APPL_TRC
            (
                "Len: 0x%02X,  CTE Type: 0x%02X\n",
                (UCHAR)sizeof(UINT8), bc_source_config.cte_type
            );
            APPL_TRC
            (
                "Len: 0x%02X,  Data Status: 0x%02X\n",
                (UCHAR)sizeof(UINT8), bc_source_config.data_status
            );
            APPL_TRC("Data Length: 0x%02X\n", bc_source_config.datalen);
            APPL_TRC("Data:\n");

            if (GA_SUCCESS == GA_broadcast_decode_basic_announcement_info
                              (
                                  bc_source_config.data,
                                  &delay,
                                  &n_sg,
                                  &parsed_len
                              ))
            {
                APPL_TRC
                (
                    "Len: 0x03,  Presentation_Delay: 0x%04X\n",
                    delay
                );
                APPL_TRC
                (
                    "Len: 0x%02X,  Num_Subgroups: 0x%02X\n",
                    (UCHAR)sizeof(n_sg), n_sg
                );

                /* Presentation_Delay + Num_Subgroups is parsed */
                bc_source_config.data += parsed_len;

                while (sg_index < n_sg)
                {
                    parsed_len = 0U;
                    if (GA_SUCCESS == GA_broadcast_decode_basic_announcement_level_1
                                      (
                                          bc_source_config.data,
                                          &bc_basic_announcement_1,
                                          &n_bis,
                                          &parsed_len
                                      ))
                    {
                        APPL_TRC("Subgroups[%d]:\n", (sg_index + 1U));
                        APPL_TRC
                        (
                            "Len: 0x%02X,  Num_BIS: 0x%02X\n",
                            (UCHAR)sizeof(n_bis), n_bis
                        );
                        appl_ga_utils_bap_display_codec_id(bc_basic_announcement_1.codec);

                        APPL_TRC
                        (
                            "Len: 0x%02X,  Codec_Specific_Configuration_Length: 0x%02X\n",
                            (UCHAR)sizeof(UINT8), bc_basic_announcement_1.codec.ie_len
                        );

                        APPL_TRC
                        (
                            "Len: 0x%02X,  Codec_Specific_Configuration:\n",
                            bc_basic_announcement_1.codec.ie_len
                        );

                        appl_ga_utils_bap_parse_and_display_codec_spec_conf_ltv
                        (
                            bc_basic_announcement_1.codec.ie,
                            bc_basic_announcement_1.codec.ie_len,
                            &rem_cs_conf
                        );

                        APPL_TRC
                        (
                            "Len: 0x%02X,  Metadata_Length: 0x%02X\n",
                            (UCHAR)sizeof(UINT8), bc_basic_announcement_1.meta.length
                        );

                        if (0x00U != bc_basic_announcement_1.meta.length)
                        {
                            APPL_TRC
                            (
                                "Len: 0x%02X,  Metadata:\n",
                                bc_basic_announcement_1.meta.length
                            );

                            appl_ga_utils_bap_parse_and_display_metadata_ltv
                            (
                                bc_basic_announcement_1.meta.data,
                                bc_basic_announcement_1.meta.length,
                                NULL,
                                NULL
                            );
                        }
                        else
                        {
                            APPL_TRC
                            (
                                "Len: 0x%02X,  Metadata: NULL\n",
                                bc_basic_announcement_1.meta.length
                            );
                        }

                        bc_source_config.data += parsed_len;

                        /* Initializing number of bis to 0 for each subgroup */
                        num_bis = 0U;

                        while (num_bis < n_bis)
                        {
                            parsed_len = 0U;
                            GA_mem_set
                            (
                                &bc_basic_announcement_2,
                                0x00U,
                                sizeof(BAP_BASIC_ANNOUNCEMENT_LEVEL_2)
                            );

                            if (GA_SUCCESS == GA_broadcast_decode_basic_announcement_level_2
                                              (
                                                  bc_source_config.data,
                                                  &bc_basic_announcement_2,
                                                  &parsed_len
                                              ))
                            {
                                num_bis++;
                                APPL_TRC
                                (
                                    "Len: 0x%02X,  BIS_index: 0x%02X\n",
                                    (UCHAR)sizeof(UINT8), bc_basic_announcement_2.stream_index
                                );

                                APPL_TRC
                                (
                                    "Len: 0x%02X,  Codec_Specific_Configuration_Length: 0x%02X\n",
                                    (UCHAR)sizeof(UINT8), bc_basic_announcement_2.stream.length
                                );

                                APPL_TRC
                                (
                                    "Len: 0x%02X,  Codec_Specific_Configuration:\n",
                                    bc_basic_announcement_2.stream.length
                                );

                                appl_ga_utils_bap_parse_and_display_codec_spec_conf_ltv
                                (
                                    bc_basic_announcement_2.stream.value,
                                    bc_basic_announcement_2.stream.length,
                                    &rem_cs_conf
                                );

                                bc_source_config.data += parsed_len;
                            }
                        }
                        /* Increment the SubGroups Parsed */
                        sg_index++;
                    }
                }
            }
        }
        break;

    case GA_BC_SOURCE_STREAM_IND:
        if (GA_SUCCESS == GA_broadcast_decode_event_params
                          (
                              GA_BC_SOURCE_STREAM_IND,
                              ga_data,
                              &bc_source_stream
                          ))
        {
            if (appl_bcast_stream_synchandle == appl_bcast_synchandle)
            {
                /* Skip printing of this received event multiple times */
                break;
            }

            appl_bcast_stream_synchandle = appl_bcast_synchandle;

            GA_CB_ENTRY("BC CAP Server");

            appl_ga_utils_bc_display_sink_console_header(ga_device, ga_status);
            APPL_TRC
            (
                "[Event]       : GA_BC_SOURCE_STREAM_IND (0x%02X)\n",
                ga_event
            );
            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();

            APPL_TRC("Data Length: 0x%02X\n", ga_datalen);
            APPL_TRC("Data:\n");
            APPL_TRC
            (
                "Len: 0x%02X,  Sync Handle: 0x%04X\n",
                (UCHAR)sizeof(UINT16), bc_source_stream.sync_handle
            );
            APPL_TRC
            (
                "Len: 0x%02X,  Num BIS: 0x%02X\n",
                (UCHAR)sizeof(UINT8), bc_source_stream.num_bis
            );
            APPL_TRC
            (
                "Len: 0x%02X,  NSE: 0x%02X\n",
                (UCHAR)sizeof(UINT8), bc_source_stream.nse
            );
            APPL_TRC
            (
                "Len: 0x%02X,  ISO Interval: 0x%04X\n",
                (UCHAR)sizeof(UINT16), bc_source_stream.iso_interval
            );
            APPL_TRC
            (
                "Len: 0x%02X,  BN: 0x%02X\n",
                (UCHAR)sizeof(UINT8), bc_source_stream.bn
            );
            APPL_TRC
            (
                "Len: 0x%02X,  PTO: 0x%02X\n",
                (UCHAR)sizeof(UINT8), bc_source_stream.pto
            );
            APPL_TRC
            (
                "Len: 0x%02X,  IRC: 0x%02X\n",
                (UCHAR)sizeof(UINT8), bc_source_stream.irc
            );
            APPL_TRC
            (
                "Len: 0x%02X,  Max PDU: 0x%04X\n",
                (UCHAR)sizeof(UINT16), bc_source_stream.max_pdu
            );
            APPL_TRC
            (
                "Len: 0x%02X,  SDU Interval: 0x%08X\n",
                (UCHAR)sizeof(UINT32), bc_source_stream.sdu_interval
            );
            APPL_TRC
            (
                "Len: 0x%02X,  Max SDU: 0x%04X\n",
                (UCHAR)sizeof(UINT16), bc_source_stream.max_sdu
            );
            APPL_TRC
            (
                "Len: 0x%02X,  Phy: 0x%02X\n",
                (UCHAR)sizeof(UCHAR), bc_source_stream.phy
            );
            APPL_TRC
            (
                "Len: 0x%02X,  Framing: 0x%02X\n",
                (UCHAR)sizeof(UCHAR), bc_source_stream.framing
            );
            APPL_TRC
            (
                "Len: 0x%02X,  Encryption: 0x%02X\n",
                (UCHAR)sizeof(UCHAR), bc_source_stream.encryption
            );
            encryption_state = bc_source_stream.encryption;

            /*
             * TODO: Validate if we already have the broadcast code via OOB.
             * If yes, broadcast code will be user input in case 94.
             */
        }
        break;

    case GA_BC_DISSOCIATE_IND:
        GA_CB_ENTRY("BC CAP Server");
        appl_ga_utils_bc_display_sink_console_header(ga_device, ga_status);
        APPL_TRC
        (
            "[Event]       : GA_BC_DISSOCIATE_IND (0x%02X)\n",
            ga_event
        );
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();

        APPL_TRC("Data Length: 0x%02X\n", ga_datalen);
        APPL_TRC("Data:\n");
        if (GA_SUCCESS == GA_broadcast_decode_event_params
                          (
                              GA_BC_DISSOCIATE_IND,
                              ga_data,
                              &bc_dissociate
                          ))
        {
            APPL_TRC
            (
                "Len: 0x%02X,  Sync Handle: 0x%04X\n",
                (UCHAR)sizeof(UINT16), bc_dissociate.sync_handle
            );
            appl_bcast_synchandle = 0xFFFF;
            appl_bcast_config_synchandle = 0xFFFF;
            appl_bcast_stream_synchandle = 0xFFFF;
        }

        break;

    case GA_BC_DISABLE_IND:
        GA_CB_ENTRY("BC CAP Server");
        appl_ga_utils_bc_display_sink_console_header(ga_device, ga_status);
        APPL_TRC
        (
            "[Event]       : GA_BC_DISABLE_IND (0x%02X)\n",
            ga_event
        );
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();

        if (GA_SUCCESS == ga_status)
        {
            APPL_TRC("Data Length: 0x%02X\n", ga_datalen);
            APPL_TRC("Data:\n");
            if (GA_SUCCESS ==
                GA_broadcast_decode_event_params(GA_BC_DISABLE_IND, ga_data, &bc_disable))
            {
                APPL_TRC
                (
                    "Len: 0x%02X,  BIG Handle: 0x%02X\n",
                    (UCHAR)sizeof(UINT8), bc_disable.big_handle
                );

                appl_bcast_config_synchandle = 0xFFFF;
                appl_bcast_stream_synchandle = 0xFFFF;
                appl_bcast_enable_synchandle = 0xFFFF;
            }

#ifdef APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA
            audio_stop_pl(AUDIO_EP_SINK);
#endif /* APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA */
        }
        break;


    case GA_BC_ASSOCIATE_CNF:
        GA_CB_ENTRY("BC CAP Server");
        appl_ga_utils_bc_display_sink_console_header(ga_device, ga_status);
        APPL_TRC
        (
            "[Event]       : GA_BC_ASSOCIATE_CNF (0x%02X)\n",
            ga_event
        );
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();

        APPL_TRC("Data Length: 0x%02X\n", ga_datalen);
        if (GA_SUCCESS == GA_broadcast_decode_event_params
                          (
                              GA_BC_ASSOCIATE_CNF,
                              ga_data,
                              &bc_associate
                          ))
        {
            APPL_TRC("Data:\n");
            APPL_TRC
            (
                "Len: 0x%02X,  Error: 0x%02X\n",
                (UCHAR)sizeof(UINT8), bc_associate.error_code
            );
            APPL_TRC
            (
                "Len: 0x%02X,  Sync Handle: 0x%04X\n",
                (UCHAR)sizeof(UINT16), bc_associate.sync_handle
            );
            appl_bcast_synchandle = bc_associate.sync_handle;
            APPL_TRC
            (
                "Len: 0x%02X,  SID: 0x%02X\n",
                (UCHAR)sizeof(UINT8), bc_associate.advertising_sid
            );
            APPL_TRC
            (
                "Len: 0x%02X,  Advertiser Address Type: 0x%02X\n",
                (UCHAR)sizeof(UINT8),
                bc_associate.advertiser_addr_type
            );
            APPL_TRC
            (
                "Len: 0x%02X,  Advertiser Address: %02X:%02X:%02X:%02X:%02X:%02X\n",
                GA_BD_ADDR_SIZE,
                bc_associate.advertiser_addr[0],
                bc_associate.advertiser_addr[1],
                bc_associate.advertiser_addr[2],
                bc_associate.advertiser_addr[3],
                bc_associate.advertiser_addr[4],
                bc_associate.advertiser_addr[5]
            );
            APPL_TRC
            (
                "Len: 0x%02X,  Adv Phy: 0x%02X\n",
                (UCHAR)sizeof(UINT8),
                bc_associate.advertiser_phy
            );
            APPL_TRC
            (
                "Len: 0x%02X,  PA Interval: 0x%04X\n",
                (UCHAR)sizeof(UINT16), bc_associate.periodic_adv_interval
            );
            APPL_TRC
            (
                "Len: 0x%02X,  Adv Clock Accuracy: 0x%02X\n",
                (UCHAR)sizeof(UINT8),
                bc_associate.advertiser_clock_accuracy
            );
        }
        break;

    case GA_BC_DISSOCIATE_CNF:
        GA_CB_ENTRY("BC CAP Server");
        appl_ga_utils_bc_display_sink_console_header(ga_device, ga_status);
        APPL_TRC
        (
            "[Event]       : GA_BC_DISSOCIATE_CNF (0x%02X)\n",
            ga_event
        );
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();

        APPL_TRC("Data Length: 0x%02X\n", ga_datalen);
        APPL_TRC("Data:\n");
        if (GA_SUCCESS == GA_broadcast_decode_event_params
                          (
                              GA_BC_DISSOCIATE_CNF,
                              ga_data,
                              &bc_dissociate
                          ))
        {
            APPL_TRC
            (
                "Len: 0x%02X,  Sync Handle: 0x%04X\n",
                (UCHAR)sizeof(UINT16), bc_dissociate.sync_handle
            );

            appl_bcast_synchandle = 0xFFFF;
            appl_bcast_config_synchandle = 0xFFFF;
            appl_bcast_stream_synchandle = 0xFFFF;
        }
        break;

    case GA_BC_ENABLE_CNF:
        GA_CB_ENTRY("BC CAP Server");
        appl_ga_utils_bc_display_sink_console_header(ga_device, ga_status);
        APPL_TRC
        (
            "[Event]       : GA_BC_ENABLE_CNF (0x%02X)\n",
            ga_event
        );
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();

        APPL_TRC("Data Length: 0x%02X\n", ga_datalen);
        APPL_TRC("Data:\n");

        if (GA_SUCCESS ==
            GA_broadcast_decode_event_params(GA_BC_ENABLE_CNF, ga_data, &bc_enable))
        {
            APPL_TRC
            (
                "Len: 0x%02X,  Error Code: 0x%02X\n",
                (UCHAR)sizeof(bc_enable.error_code),
                bc_enable.error_code
            );
            APPL_TRC
            (
                "Len: 0x%02X,  BIG Handle: 0x%02X\n",
                (UCHAR)sizeof(bc_enable.big_handle),
                bc_enable.big_handle
            );
            APPL_TRC
            (
                "Len: 0x%02X,  Transport Latency BIG: 0x%08X\n",
                (UCHAR)sizeof(bc_enable.transport_latency_big),
                bc_enable.transport_latency_big
            );
            APPL_TRC
            (
                "Len: 0x%02X,  NSE: 0x%02X\n",
                (UCHAR)sizeof(bc_enable.nse), bc_enable.nse
            );
            APPL_TRC
            (
                "Len: 0x%02X,  BN: 0x%02X\n",
                (UCHAR)sizeof(bc_enable.bn), bc_enable.bn
            );
            APPL_TRC
            (
                "Len: 0x%02X,  PTO: 0x%02X\n",
                (UCHAR)sizeof(bc_enable.pto), bc_enable.pto
            );
            APPL_TRC
            (
                "Len: 0x%02X,  IRC: 0x%02X\n",
                (UCHAR)sizeof(bc_enable.irc), bc_enable.irc
            );
            APPL_TRC
            (
                "Len: 0x%02X,  Max PDU: 0x%04X\n",
                (UCHAR)sizeof(bc_enable.max_pdu), bc_enable.max_pdu
            );
            APPL_TRC
            (
                "Len: 0x%02X,  ISO Interval: 0x%04X\n",
                (UCHAR)sizeof(bc_enable.iso_interval), bc_enable.iso_interval
            );

            APPL_TRC
            (
                "Len: 0x%02X,  Number of BISes: 0x%02X\n",
                (UCHAR)sizeof(bc_enable.num_bis), bc_enable.num_bis
            );
            for (i = 0U; i < bc_enable.num_bis; i++)
            {
                APPL_TRC("BIS[%d]:\n", (i + 1U));
                APPL_TRC
                (
                    "\tLen: 0x%02X,  BIS Conn Handle: 0x%04X\n",
                    (UCHAR)sizeof(bc_enable.connection_handle[i]),
                    bc_enable.connection_handle[i]
                );
            }

            if (0 < bc_enable.num_bis)
            {
                appl_bcast_enable_synchandle = appl_bcast_synchandle;

#if 0
                /* Broadcast Sink Dissociate: Disable PA Scan, Stop rx PA reports */
                printf("Dissociate Broadcast Sink...\n");
                retval = GA_broadcast_dissociate
                (
                    appl_bcast_synchandle
                );
                printf("Retval - 0x%04X\n", retval);

                /*  Broadcast Sink Release: Disable EA Scan */
                printf("Release Broadcast Sink...\n");
                retval = GA_broadcast_scan_end();
                printf("Retval - 0x%04X\n", retval);
#endif /* 0 */

                isISODataPathReset = GA_TRUE;
                appl_ga_prep_bcast_for_streaming(0x00, bc_enable.connection_handle[0]);

#ifdef APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA
                audio_start_pl(AUDIO_EP_SINK);
#endif /* APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA */
            }
        }
        break;

    case GA_BC_DISABLE_CNF:
        GA_CB_ENTRY("BC CAP Server");
        appl_ga_utils_bc_display_sink_console_header(ga_device, ga_status);
        APPL_TRC
        (
            "[Event]       : GA_BC_DISABLE_CNF (0x%02X)\n",
            ga_event
        );
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();

        if (GA_SUCCESS == ga_status)
        {
            APPL_TRC("Data Length: 0x%02X\n", ga_datalen);
            APPL_TRC("Data:\n");
            if (GA_SUCCESS ==
                GA_broadcast_decode_event_params(GA_BC_DISABLE_CNF, ga_data, &bc_disable))
            {
                APPL_TRC
                (
                    "Len: 0x%02X,  BIG Handle: 0x%02X\n",
                    (UCHAR)sizeof(UINT8), bc_disable.big_handle
                );

                appl_bcast_config_synchandle = 0xFFFF;
                appl_bcast_stream_synchandle = 0xFFFF;
                appl_bcast_enable_synchandle = 0xFFFF;
            }

#ifdef APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA
            audio_stop_pl(AUDIO_EP_SINK);
#endif /* APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA */
        }
        break;

    default:
        APPL_TRC("Received UnHandled Event\n");
        break;
    }

    /* GA_CB_EXIT(); */

    return retval;
}

#endif /* BAP_BROADCAST */

#ifdef INCLUDE_ETHERMIND_LC3
GA_RESULT appl_ga_lc3_decode_n_play(UCHAR* data, UINT16 datalen)
{
    INT32 encoded_byte_len[APPL_LC3_CHANNELS_MAX];
    static INT16  dec16_buffer[LC3_INPUT_FRAME_SIZE_MAX];
    static INT16  dec16_silence[LC3_INPUT_FRAME_SIZE_MAX];
    INT32 flg_bytes[APPL_LC3_CHANNELS_MAX];
    UINT16 index;
    INT32 samples_per_frame;

    index = 0;
    samples_per_frame = 0;

    /* Initialize */
    index = 0;

#ifdef DUMP_ISO_DATA_TO_FILE
    if (GA_TRUE == is_dump_enabled)
    {
        if (NULL != lc3_dec_pre_media_dump_fd)
        {
            fwrite(data, sizeof(UINT8), (datalen), lc3_dec_pre_media_dump_fd);
        }
    }
#endif /* DUMP_ISO_DATA_TO_FILE */

    BT_mem_copy(enc_buffer[0], data, datalen);

    flg_bytes[0] = 0;
    encoded_byte_len[0] = datalen; /* Single Channel Only */

    samples_per_frame = LC3_decoder_get_frame_length(&lc3_dec_ctx);

    if (LC3_DECODER_SUCCESS == LC3_decoder_process(&lc3_dec_ctx, flg_bytes, encoded_byte_len))
    {
        /* APPL_TRC("ISO: LC3 Decoded Data : %d\n", samples_per_frame); */

        for (index = 0; index < samples_per_frame; index++)
        {
            dec16_buffer[index] = (INT16)(dec_buffer[0][index]);
        }

#ifdef DUMP_ISO_DATA_TO_FILE
        if (GA_TRUE == is_dump_enabled)
        {
            if (NULL != lc3_dec_media_dump_fd)
            {
                fwrite(dec16_buffer, sizeof(INT16), (samples_per_frame), lc3_dec_media_dump_fd);
            }
        }
#endif /* DUMP_ISO_DATA_TO_FILE */

#ifdef APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA
        if (isISODataPathReset)
        {
            GA_mem_set(dec16_silence, 0x00, sizeof(dec16_silence));

            for (index = 0; index < APPL_GA_LC3_SILENCE_BUFFER_COUNT; index++)
            {
                audio_write_pl
                (
                    AUDIO_EP_SINK,
                    (UCHAR*)dec16_silence,
                    (UINT16)(samples_per_frame << 1) /* Check this */
                );
            }

            isISODataPathReset = GA_FALSE;
        }

        audio_write_pl
        (
            AUDIO_EP_SINK,
            (UCHAR*)dec16_buffer,
            (UINT16)(samples_per_frame << 1) /* Check this */
        );
#endif /* APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA */
    }

    return GA_SUCCESS;
}
#endif /* INCLUDE_ETHERMIND_LC3 */

void appl_ga_init(void)
{

#ifdef APPL_GA_SAMPLE_APP_UCC
    appl_ga_automate_unicast_client_operations();
#endif /* APPL_GA_SAMPLE_APP_UCC */

#ifdef APPL_GA_SAMPLE_APP_UCS
    appl_ga_automate_unicast_server_operations();
#endif /* APPL_GA_SAMPLE_APP_UCS */

    return;

}
int main_ga_legacy_operations(void)
{
    GA_RESULT retval;
    int choice;
    int menu_choice;
    UINT8 index, ase_count, ase_id, j;
    UINT8 peer_ase_count, peer_ase_id[2];
    GA_CONTENT_INFO content_info[2];
    GA_CODEC_INFO local_codec_info[2];
    UINT8 ccid[2];
    GA_ENDPOINT device;
    UINT8 local_ase_db_index;
    UCHAR bc_code[GA_BROADCAST_CODE_SIZE];
    UCHAR * bc_code_ptr;

    GA_LC3_CODEC_SPECIFIC_CONF cs_conf;

    /* Parameters related to App's choice for ISOC CIG and CIS Setup */
    UINT16 cis_connection_handle[APPL_GA_MAX_CIS_COUNT];
    UINT8 rem_ase_db_index;

    /* Parameters related to ISO Data Path */
    UCHAR  data_path_direction;

    retval = GA_FAILURE;
    local_ase_db_index = 0;
    ase_id = 0xFF;
    data_path_direction = APPL_GA_ISO_UNKNOWN;
    rem_ase_db_index = 0;
    bc_code_ptr = NULL;

    GA_LOOP_FOREVER()
    {
        printf("\n");
        printf("%s", ga_options);
        scanf("%d", &choice);

        menu_choice = choice;

        switch (choice)
        {
        case 0:
            break; /* return 0; */

        case 1:
            break;

        case 2:
            if (APPL_GA_INIT_STATUS_GA == ga_init_status)
            {
                APPL_ERR("GA is already initialized with Reference application - New !\n");
            }
            else if (APPL_GA_INIT_STATUS_LEGACY == ga_init_status)
            {
                APPL_ERR("GA is already initialized with Reference application - Legacy !\n");
            }
            else
            {
                appl_ga_register();
            }

            break;

        case 3:
            appl_ga_automate_register_pacs();
            break;

        case 4:
            appl_ga_register_ascs();
            break;

        case 5:
            /* TODO: */
            break;

        case 6:
            printf("Enter endpoint address: ");
            appl_get_bd_addr(device.bd_addr);
            printf("Enter endpoint type: ");
            scanf("%d", &choice);
            device.bd_type = (UINT8)choice;

            printf("Enter ASE Count: ");
            scanf("%d", &choice);
            ase_count = (UINT8)choice;

            for (index = 0; index < ase_count; index++)
            {
                printf("Enter ASE ID: ");
                scanf("%d", &choice);
                peer_ase_id[index] = (UINT8)choice;

                cs_conf.sf = 0x08; /* Value: 48KHz */
                cs_conf.fd = 0x01;  /* Value: 10ms Frame duration */
                cs_conf.aca = 0x01; /* Value: Channel Count 1 - Bit 0  */
                cs_conf.opcf = 0x64;  /* Value: Number of octets supported per codec frame - 100 */
                cs_conf.mcfpSDU = 0x01; /* Value - 1, by default */

                GA_mem_set(&local_codec_info[index].ie[0], 0, GA_CODEC_IE_LEN);
                appl_ga_create_codec_spec_conf_ltv(&cs_conf, local_codec_info[index].ie, &local_codec_info[index].ie_len);
            }

            retval = GA_notify_ase_configure(&device, ase_count, peer_ase_id, local_codec_info);
            printf("Retval - 0x%04X\n", retval);
            break;

        case 7:
            printf("Enter endpoint address: ");
            appl_get_bd_addr(device.bd_addr);
            printf("Enter endpoint type: ");
            scanf("%d", &choice);
            device.bd_type = (UINT8)choice;

            printf("Enter ASE Count: ");
            scanf("%d", &choice);
            ase_count = (UINT8)choice;

            for (index = 0; index < ase_count; index++)
            {
                printf("Enter ASE ID: ");
                scanf("%d", &choice);
                peer_ase_id[index] = (UINT8)choice;

            }

            retval = GA_notify_ase_receiver_start_ready(&device, ase_count, peer_ase_id);
            printf("Retval - 0x%04X\n", retval);

            if (GA_SUCCESS == retval)
            {
#ifdef APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA
                audio_start_pl(AUDIO_EP_SINK);
#endif /* APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA */

                isISODataPathReset = GA_TRUE;

#ifdef DUMP_ISO_DATA_TO_FILE
                if (GA_TRUE == is_dump_enabled)
                {
                    lc3_dec_media_dump_fd = fopen("lc3_dec_media_dump", "wb+");

                    if (NULL != lc3_dec_media_dump_fd)
                    {
                        APPL_TRC("Dec Media Dump created successfully\n");
                    }

                    lc3_dec_pre_media_dump_fd = fopen("lc3_dec_pre_media_dump", "wb+");

                    if (NULL != lc3_dec_pre_media_dump_fd)
                    {
                        APPL_TRC("Dec Media Dump created successfully\n");
                    }
                }
#endif /* DUMP_ISO_DATA_TO_FILE */
            }
            break;

        case 8:
            printf("Enter endpoint address: ");
            appl_get_bd_addr(device.bd_addr);
            printf("Enter endpoint type: ");
            scanf("%d", &choice);
            device.bd_type = (UINT8)choice;

            printf("Enter ASE Count: ");
            scanf("%d", &choice);
            ase_count = (UINT8)choice;

            for (index = 0; index < ase_count; index++)
            {
                printf("Enter ASE ID: ");
                scanf("%d", &choice);
                peer_ase_id[index] = (UINT8)choice;

            }

            retval = GA_notify_ase_suspend(&device, ase_count, peer_ase_id);
            printf("Retval - 0x%04X\n", retval);

            if (GA_SUCCESS == retval)
            {
                for (index = 0; index < ase_count; index++)
                {
                    if (GA_FAILURE == appl_ga_fetch_local_ase_db_index_from_ase_index(peer_ase_id[index], &local_ase_db_index))
                    {
                        printf("Unknown ASE ID\n");
                    }
                    else
                    {
                        ga_local_audio_ase[local_ase_db_index].curr_state = APPL_GA_STATE_DISABLING;
#ifdef APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA
                        if (GA_ROLE_SINK == ga_local_audio_ase[local_ase_db_index].codec_conf.role)
                        {
                            audio_stop_pl(AUDIO_EP_SINK);
                            ga_local_audio_ase[local_ase_db_index].curr_state = APPL_GA_STATE_SUSPENDED;
                            BT_debug_trace(BT_MODULE_ID_WT, "async APPL_GA_STATE_SUSPENDED 0x%02x\n", local_ase_db_index);
                        }
                        else if (GA_ROLE_SOURCE == ga_local_audio_ase[local_ase_db_index].codec_conf.role)
                        {
                            audio_stop_pl(AUDIO_EP_SOURCE);
                            BT_debug_trace(BT_MODULE_ID_WT, "async APPL_GA_STATE_SUSPENDED 0x%02x\n", local_ase_db_index);
                        }
#endif /* APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA */
                    }
                }
            }
            break;

        case 9:
            printf("Enter endpoint address: ");
            appl_get_bd_addr(device.bd_addr);
            printf("Enter endpoint type: ");
            scanf("%d", &choice);
            device.bd_type = (UINT8)choice;

            printf("Enter ASE Count: ");
            scanf("%d", &choice);
            ase_count = (UINT8)choice;

            for (index = 0; index < ase_count; index++)
            {
                printf("Enter ASE ID: ");
                scanf("%d", &choice);
                peer_ase_id[index] = (UINT8)choice;

                printf("Enter the Contexts: ");
                scanf("%d", &choice);
                content_info[index].contexts = (UINT16)choice;

                printf("Enter CCID Count: ");
                scanf("%d", &choice);
                content_info[index].num_ccid = (UINT8)choice;

                for (j = 0; j < content_info[index].num_ccid; j++)
                {
                    printf("Enter CCID: ");
                    scanf("%d", &choice);
                    ccid[j] = (UINT8)choice;
                }
                content_info[index].ccid = ccid;
            }

            retval = GA_notify_ase_update(&device, ase_count, peer_ase_id, content_info);
            printf("Retval - 0x%04X\n", retval);
            break;

        case 10:
            printf("Enter endpoint address: ");
            appl_get_bd_addr(device.bd_addr);
            printf("Enter endpoint type: ");
            scanf("%d", &choice);
            device.bd_type = (UINT8)choice;

            printf("Enter ASE Count: ");
            scanf("%d", &choice);
            ase_count = (UINT8)choice;

            for (index = 0; index < ase_count; index++)
            {
                printf("Enter ASE ID: ");
                scanf("%d", &choice);
                peer_ase_id[index] = (UINT8)choice;

            }

            retval = GA_notify_ase_release(&device, ase_count, peer_ase_id);
            printf("Retval - 0x%04X\n", retval);

            if (GA_SUCCESS == retval)
            {
                for (index = 0; index < ase_count; index++)
                {
                    if (GA_FAILURE == appl_ga_fetch_local_ase_db_index_from_ase_index(peer_ase_id[index], &local_ase_db_index))
                    {
                        printf("Unknown ASE ID\n");
                    }
                    else
                    {
                        ga_local_audio_ase[local_ase_db_index].curr_state = APPL_GA_STATE_DISABLING;

                        if (GA_ROLE_SOURCE == ga_local_audio_ase[local_ase_db_index].codec_conf.role)
                        {
                            data_path_direction = APPL_GA_ISO_INPUT;
#ifdef APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA
                            audio_stop_pl(AUDIO_EP_SOURCE);
#endif /* APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA */
                        }
                        else if (GA_ROLE_SINK == ga_local_audio_ase[local_ase_db_index].codec_conf.role)
                        {
                            data_path_direction = APPL_GA_ISO_OUTPUT;
#ifdef APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA
                            audio_stop_pl(AUDIO_EP_SINK);
#endif /* APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA */
                        }

                        APPL_TRC("BT_hci_le_remove_iso_data_path data_path_direction %d\n", data_path_direction);
                        retval = BT_hci_le_remove_iso_data_path
                                 (
                                     ga_local_audio_ase[local_ase_db_index].cis_conn_handle,
                                     data_path_direction
                                 );

                        if (API_SUCCESS != retval)
                        {
                            APPL_TRC("FAILED !!! Error code = %04X\n", retval);
                        }
                        else
                        {
                            APPL_TRC("API returned success...\n");

                            ga_local_audio_ase[local_ase_db_index].curr_state = APPL_GA_STATE_RELEASING;

#ifdef APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA
                            if (APPL_GA_ISO_INPUT == data_path_direction)
                            {
                                /* Setup Module for Media Data - Generate Audio Data */
                                audio_shutdown_pl();

#ifdef INCLUDE_ETHERMIND_LC3
                                /* De-init LC3 Module */
                                LC3_encoder_delete(&lc3_enc_ctx);
                                appl_ga_lc3_enc_ready = GA_FALSE;
#endif /* INCLUDE_ETHERMIND_LC3 */
                            }
                            else if (APPL_GA_ISO_OUTPUT == data_path_direction)
                            {
                                /* Setup Module for Media Data - Play Audio Data */
                                audio_shutdown_pl();

#ifdef INCLUDE_ETHERMIND_LC3
                                /* De-init LC3 Module */
                                LC3_decoder_delete(&lc3_dec_ctx);
                                appl_ga_lc3_dec_ready = GA_FALSE;
#endif /* INCLUDE_ETHERMIND_LC3 */
                            }
#endif /* APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA */
                        }
                    }
                }
            }
            break;

        case 11:
            printf("Enter endpoint address: ");
            appl_get_bd_addr(device.bd_addr);
            printf("Enter endpoint type: ");
            scanf("%d", &choice);
            device.bd_type = (UINT8)choice;

            printf("Enter ASE Count: ");
            scanf("%d", &choice);
            ase_count = (UINT8)choice;

            for (index = 0; index < ase_count; index++)
            {
                printf("Enter ASE ID: ");
                scanf("%d", &choice);
                peer_ase_id[index] = (UINT8)choice;
            }

            retval = GA_notify_release_complete(&device, ase_count, peer_ase_id);
            printf("Retval - 0x%04X\n", retval);
            break;

        case 20:
            appl_ga_unicast_client_setup();
            break;

        case 21:
        {
            CONSOLE_OUT("\n1. Release\n2. Close\n");
            CONSOLE_OUT("Enter your choice: ");
            CONSOLE_IN("%d", &choice);
            if (1 == choice)
            {
                APPL_TRC("GA_release\n");
                retval = GA_release(&ga_rem_dev_info[curr_rem_index].ga_context);
                APPL_TRC("Retval - 0x%04X\n", retval);
            }
            else if (2 == choice)
            {
                APPL_TRC("GA_close\n");
                retval = GA_close(&ga_rem_dev_info[curr_rem_index].ga_context);
                APPL_TRC("Retval - 0x%04X\n", retval);
            }
            else
            {
                APPL_ERR("Invalid choice !\n");
            }
        }
            break;

        case 22:
            appl_ga_unicast_client_get_pacs_capabilities();
            break;

        case 25:
            appl_ga_unicast_client_discover_ascs();
            break;

        case 26:
            appl_ga_unicast_client_configure_codec();
            break;

        case 27:
            appl_ga_unicast_client_setup_cig();
            break;

        case 28:
            appl_ga_unicast_client_configure_qos();
            break;

        case 29:
            appl_ga_unicast_client_enable();
            break;

        case 30:
            appl_ga_unicast_client_create_cis();
            break;

        case 31:
            printf("Enter ASE Count: ");
            scanf("%d", &choice);
            peer_ase_count = (UINT8)choice;

            for (index = 0; index < peer_ase_count; index++)
            {
                printf("Enter ASE ID: ");
                scanf("%d", &choice);
                peer_ase_id[index] = (UINT8)choice;

                if (GA_FAILURE == appl_ga_fetch_rem_ase_db_index_from_ase_id(peer_ase_id[index], &rem_ase_db_index))
                {
                    printf("Unknown ASE ID\n");
                }
            }

            printf("Receiver Start Ready on SEP\n");
            retval = GA_sep_receiver_start_ready(&ga_rem_dev_info[curr_rem_index].ga_context, peer_ase_count, peer_ase_id);
            printf("Retval - 0x%04X\n", retval);
            break;

        case 32:
            printf("Enter ASE Count: ");
            scanf("%d", &choice);
            peer_ase_count = (UINT8)choice;

            for (index = 0; index < peer_ase_count; index++)
            {
                printf("Enter ASE ID: ");
                scanf("%d", &choice);
                peer_ase_id[index] = (UINT8)choice;

                if (GA_FAILURE == appl_ga_fetch_rem_ase_db_index_from_ase_id(peer_ase_id[index], &rem_ase_db_index))
                {
                    printf("Unknown ASE ID\n");
                }

                printf("Enter the Contexts: ");
                scanf("%d", &choice);
                content_info[index].contexts = (UINT16)choice;

                printf("Enter CCID Count: ");
                scanf("%d", &choice);
                content_info[index].num_ccid = (UINT8)choice;

                for (j = 0; j < content_info[index].num_ccid; j++)
                {
                    printf("Enter CCID: ");
                    scanf("%d", &choice);
                    ccid[j] = (UINT8)choice;
                }
                content_info[index].ccid = ccid;
            }

            printf("Update Codec on SEP\n");
            retval = GA_sep_update(&ga_rem_dev_info[curr_rem_index].ga_context, peer_ase_count, peer_ase_id, content_info);
            printf("Retval - 0x%04X\n", retval);
            break;

        case 33:
            printf("Enter ASE Count: ");
            scanf("%d", &choice);
            peer_ase_count = (UINT8)choice;

            for (index = 0; index < peer_ase_count; index++)
            {
                printf("Enter ASE ID: ");
                scanf("%d", &choice);
                peer_ase_id[index] = (UINT8)choice;

                if (GA_FAILURE == appl_ga_fetch_rem_ase_db_index_from_ase_id(peer_ase_id[index], &rem_ase_db_index))
                {
                    printf("Unknown ASE ID\n");
                }
            }

            printf("Suspend Codec on SEP\n");
            retval = GA_sep_suspend(&ga_rem_dev_info[curr_rem_index].ga_context, peer_ase_count, peer_ase_id);
            printf("Retval - 0x%04X\n", retval);

            if (API_SUCCESS != retval)
            {
                APPL_TRC("FAILED !!! Error code = %04X\n", retval);
            }
            else
            {
                APPL_TRC("API returned success...\n");
            }
            break;

        case 34:
            printf("Enter ASE Count: ");
            scanf("%d", &choice);
            peer_ase_count = (UINT8)choice;

            for (index = 0; index < peer_ase_count; index++)
            {
                printf("Enter ASE ID: ");
                scanf("%d", &choice);
                peer_ase_id[index] = (UINT8)choice;

                if (GA_FAILURE == appl_ga_fetch_rem_ase_db_index_from_ase_id(peer_ase_id[index], &rem_ase_db_index))
                {
                    printf("Unknown ASE ID\n");
                }
            }

            printf("Receiver Stop Ready on SEP\n");
            retval = GA_sep_receiver_stop_ready(&ga_rem_dev_info[curr_rem_index].ga_context, peer_ase_count, peer_ase_id);
            printf("Retval - 0x%04X\n", retval);
            break;

        case 35:
            printf("Enter ASE Count: ");
            scanf("%d", &choice);
            peer_ase_count = (UINT8)choice;

            for (index = 0; index < peer_ase_count; index++)
            {
                printf("Enter ASE ID: ");
                scanf("%d", &choice);
                peer_ase_id[index] = (UINT8)choice;

                if (GA_FAILURE == appl_ga_fetch_rem_ase_db_index_from_ase_id(peer_ase_id[index], &rem_ase_db_index))
                {
                    printf("Unknown ASE ID\n");
                }
            }

            printf("Release Codec on SEP\n");
            retval = GA_sep_release(&ga_rem_dev_info[curr_rem_index].ga_context, peer_ase_count, peer_ase_id);
            printf("Retval - 0x%04X\n", retval);

            if (API_SUCCESS != retval)
            {
                APPL_TRC("FAILED !!! Error code = %04X\n", retval);
            }
            else
            {
                APPL_TRC("API returned success...\n");
            }
            break;

        case 36:
            APPL_TRC("Enter cis_connection_handle :\n");
            scanf("%x", &choice);

            APPL_TRC("ISO Data Write: \n");
            retval = BT_hci_iso_write
                     (
                         (UINT8)choice,
                         0, /* ts_flag */
                         0, /* ts */
                         0, /* Seq num */
                         &appl_ga_iso_dummy_data[0],
                         APPL_GA_ISO_DUMMY_MAX_DATALEN
                     );

            if (API_SUCCESS != retval)
            {
                APPL_TRC("FAILED !!! Error code = %04X\n", retval);
            }
            else
            {
                APPL_TRC("API returned success...\n");
            }
            break;

        case 37:
            APPL_TRC("Enter cis_connection_handle :\n");
            scanf("%x", &choice);
            cis_connection_handle[0] = (UINT16)choice;

            APPL_TRC("Enter data_path_direction :(0 - Tx, 1 - Rx)\n");
            scanf("%x", &choice);
            data_path_direction = (UCHAR)choice;

            APPL_TRC("Removing ISO Data Path:\n");
            APPL_TRC("BT_hci_le_remove_iso_data_path data_path_direction %d\n", data_path_direction);
            retval = BT_hci_le_remove_iso_data_path
                     (
                         cis_connection_handle[0],
                         data_path_direction
                     );

            if (API_SUCCESS != retval)
            {
                APPL_TRC("FAILED !!! Error code = %04X\n", retval);
            }
            else
            {
                APPL_TRC("API returned success...\n");

#ifdef APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA
                if (2 == data_path_direction)
                {
                    /* Setup Module for Media Data - Generate Audio Data */
                    audio_shutdown_pl();

#ifdef INCLUDE_ETHERMIND_LC3
                    /* De-init LC3 Module */
                    LC3_encoder_delete(&lc3_enc_ctx);
                    appl_ga_lc3_enc_ready = GA_FALSE;
#endif /* INCLUDE_ETHERMIND_LC3 */
                }
                else if (1 == data_path_direction)
                {
                    /* Setup Module for Media Data - Play Audio Data */
                    audio_shutdown_pl();

#ifdef INCLUDE_ETHERMIND_LC3
                    /* De-init LC3 Module */
                    LC3_decoder_delete(&lc3_dec_ctx);
                    appl_ga_lc3_dec_ready = GA_FALSE;
#endif /* INCLUDE_ETHERMIND_LC3 */
                }
#endif /* APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA */
            }
            break;

        case 38:
            APPL_TRC("Enter connection_handle :\n");
            scanf("%x", &choice);
            cis_connection_handle[0] = (UINT16)choice;

            APPL_TRC("Accept CIS:\n");
            retval = BT_hci_le_accept_cis_request
                     (
                         cis_connection_handle[0]
                     );

            if (API_SUCCESS != retval)
            {
                APPL_TRC("FAILED !!! Error code = %04X\n", retval);
            }
            else
            {
                APPL_TRC("API returned success...\n");
            }
            break;

        case 15:
#ifdef DUMP_ISO_DATA_TO_FILE
            if (GA_TRUE == is_dump_enabled)
            {
                if (NULL != lc3_enc_media_dump_fd)
                {
                    fclose(lc3_enc_media_dump_fd);
                    lc3_enc_media_dump_fd = NULL;
                }
                if (NULL != lc3_dec_media_dump_fd)
                {
                    fclose(lc3_dec_media_dump_fd);
                    lc3_dec_media_dump_fd = NULL;
                }
                if (NULL != lc3_dec_pre_media_dump_fd)
                {
                    fclose(lc3_dec_pre_media_dump_fd);
                    lc3_dec_pre_media_dump_fd = NULL;
                }
                if (NULL != lc3_enc_pre_media_dump_fd)
                {
                    fclose(lc3_enc_pre_media_dump_fd);
                    lc3_enc_pre_media_dump_fd = NULL;
                }
                if (NULL != lc3_enc_src_enqueue_fd)
                {
                    fclose(lc3_enc_src_enqueue_fd);
                    lc3_enc_src_enqueue_fd = NULL;
                }
                if (NULL != lc3_audio_pl_src_callback_dump_fd)
                {
                    fclose(lc3_audio_pl_src_callback_dump_fd);
                    lc3_audio_pl_src_callback_dump_fd = NULL;
                }
            }
#endif /* DUMP_ISO_DATA_TO_FILE */
            break;

        case 16:
            /* TDODO */
            break;

        case 17:
            /* TODO */
            break;

        case 40:
            appl_vcp_ct_operations();
            break;

        case 41:
            appl_vcp_rd_operations();
            break;

        case 42:
            appl_micp_ct_operations();
            break;

        case 43:
            appl_micp_dev_operations();
            break;

        case 44:
            appl_csip_sc_operations();
            break;

        case 45:
            appl_csip_sm_operations();
            break;

      /* Temp */
        case 50:
            appl_hci_le_set_advertising_enable();
            break;

        case 51:
            /* Create Connection with remote */
            appl_hci_le_connect((BT_DEVICE_ADDR *)&appl_ga_device, 0x00U);
            break;

        case 52:
            /* Create Extended Connection with remote */
            appl_hci_le_connect((BT_DEVICE_ADDR *)&appl_ga_device, 0x01U);
            break;

        case 55:
            appl_ga_unicast_auth_and_encrypt();
            break;

        case 60:
            appl_ga_unicast_set_remote_dev();
            break;

#ifdef BAP_BROADCAST
        case 80:
            printf ("Allocating Broadcast Session...\n");
            retval = GA_broadcast_alloc_session
                     (
                         &appl_bcast_ssn
                     );
            if (GA_SUCCESS == retval)
            {
                curr_GATT_role = APPL_GA_GATT_NONE;
            }
            printf ("Retval - 0x%04X\n", retval);
            break;

        case 81:
            printf ("Freeing Broadcast Session...\n");
            retval = GA_broadcast_free_session
                     (
                         appl_bcast_ssn
                     );
            printf ("Retval - 0x%04X\n", retval);
            break;

        case 82:
            CONSOLE_OUT("Select your choice:\n1. UnEncrypted Stream\n2. Encrypted Stream\n");
            CONSOLE_IN("%d", &choice);
            if (1 == choice)
            {
                APPL_TRC("Configuring unencrypted broadcast\n");
            }
            else
            {
                APPL_TRC("Configuring encrypted broadcast\n");
                bc_code_ptr = &appl_bcast_code[0U];
            }
            printf ("Configuring Broadcast Session...\n");
            retval = GA_broadcast_configure_session
                     (
                         appl_bcast_ssn,
                         appl_bcast_phy,
                         appl_bcast_packing,
                         bc_code_ptr
                     );
            printf ("Retval - 0x%04X\n", retval);
            break;

        case 83:
            printf ("Registering SEP to Broadcast Session...\n");
            retval = GA_broadcast_register_sep
                     (
                         appl_bcast_ssn,
                         &appl_bcast_codecinfo,
                         &appl_bcast_metainfo,
                         appl_bcast_nstream,
                         &appl_bcast_stream_codec_ie,
                         &appl_bcast_sep
                     );
            printf ("Retval - 0x%04X\n", retval);
            break;

        case 84:
#ifdef GA_PBP
        {
            UCHAR sdata[32], adata[32];
            UINT16 sdatalen, adatalen;
            UCHAR type;

            /* Form the service data */
            /*
             * As per PBP Spec, A PBS should include the Program_Info
             * length-type-value (LTV) structure metadata (defined in
             * Bluetooth Assigned Numbers [2]) to help users determine
             * which broadcast Audio Stream to select when populating
             * the BASE
             */
            PBP_ENCODE_BCAST_ADV_SERVICE_DATA_WITH_METADATA
            (
                sdata,
                sdatalen,
                PBP_BCAST_ENCRYPTION_OFF,
                PBP_BCAST_SQ_OFF,
                PBP_BCAST_HQ_ON,
                appl_bcast_metainfo.length,
                &appl_bcast_metainfo.data
            );

            /* Form the additional Adv data */
            PBP_ENCODE_BCAST_ADV_BROADCAST_NAME(adata, adatalen, "Gate-3", 7U);

            printf("Registering Advset to Session...\n");
            retval = GA_broadcast_register_advset_info
                     (
                         appl_bcast_ssn,
                         1U,
                         sdata,
                         sdatalen,
                         adata,
                         adatalen
                     );
            printf("Retval - 0x%04X\n", retval);

            type = GA_BCAST_SIG_PROFILE_PUBLIC_BROADCAST;
#else /* GA_PBP */

            printf("Registering Advset to Session...\n");
            retval = GA_broadcast_register_advset(appl_bcast_ssn, 1U);
            printf("Retval - 0x%04X\n", retval);

            type = GA_BCAST_SIG_PROFILE_NONE;
#endif /* GA_PBP */

            printf ("Setup Broadcast Session...\n");
            retval = GA_broadcast_setup_announcement
                     (
                         appl_bcast_ssn,
                         appl_bcast_id,
                         type,
                         appl_bcast_presentation_delay
                     );
            printf ("Retval - 0x%04X\n", retval);
        }
            break;

        case 85:
            printf ("Release Broadcast Session...\n");
            retval = GA_broadcast_end_announcement
                     (
                         appl_bcast_ssn
                     );
            printf ("Retval - 0x%04X\n", retval);
            break;

        case 86:
            printf ("Start Broadcast Session...\n");
            retval = GA_broadcast_start
                     (
                         appl_bcast_ssn,
                         appl_bcast_sdu_interval,
                         appl_bcast_max_sdu,
                         appl_bcast_max_latency,
                         appl_bcast_rtn,
                         appl_bcast_framing
                     );
            printf ("Retval - 0x%04X\n", retval);
            break;

        case 87:
#ifdef APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA
            audio_stop_pl(AUDIO_EP_SOURCE);
#endif /* APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA */

            printf ("Suspend Broadcast Session...\n");
            retval = GA_broadcast_suspend
                     (
                         appl_bcast_ssn
                     );
            printf ("Retval - 0x%04X\n", retval);
            break;

        case 90:
            CONSOLE_OUT("Address of interested Broadcast Source\n1. Yes\n2. No\n");
            CONSOLE_IN("%d", &choice);
            if (1 == choice)
            {
                CONSOLE_OUT("Enter endpoint address: ");
                appl_get_bd_addr(bc_src_interested.bd_addr);
                CONSOLE_OUT("Enter endpoint type: ");
                CONSOLE_IN("%d", &choice);
                bc_src_interested.bd_type = (UINT8)choice;
            }
            else
            {
                GA_mem_set
                (
                    &bc_src_interested.bd_addr,
                    0U,
                    sizeof(GA_BD_ADDR_SIZE)
                );
                bc_src_interested.bd_type = 0U;
            }

            printf ("Setup Broadcast Sink...\n");
            retval = GA_broadcast_scan_announcement();
            if (GA_SUCCESS == retval)
            {
                curr_GATT_role = APPL_GA_GATT_NONE;
            }
            printf ("Retval - 0x%04X\n", retval);
            break;

        case 91:
            printf ("Release Broadcast Sink...\n");
            retval = GA_broadcast_scan_end();
            printf ("Retval - 0x%04X\n", retval);
            break;

        case 92:
            printf("Enter SID: ");
            scanf("%d", &choice);
            appl_bcast_source_sid = (UINT8)choice;

            printf("Enter endpoint address: ");
            appl_get_bd_addr(appl_ga_device.bd_addr);
            printf("Enter endpoint type: ");
            scanf("%d", &choice);
            appl_ga_device.bd_type = (UINT8)choice;

            printf ("Associate Broadcast Sink...\n");
            retval = GA_broadcast_associate
                     (
                         appl_bcast_source_sid,
                         &appl_ga_device
                     );
            printf ("Retval - 0x%04X\n", retval);
            break;

        case 93:
            printf ("Dissociate Broadcast Sink...\n");
            retval = GA_broadcast_dissociate
                     (
                         appl_bcast_synchandle
                     );
            printf ("Retval - 0x%04X\n", retval);
            break;

        case 94:
            /* This is simulation of OOB to use the BC Code */
            if (GA_TRUE == encryption_state)
            {
                CONSOLE_OUT("Enter Broadcast_Code [IN HEX]: ");
                for (index = 0U; index < GA_BROADCAST_CODE_SIZE; index++)
                {
                    CONSOLE_IN("%x", &choice);
                    bc_code[index] = (UCHAR)choice;
                }
                APPL_TRC("Broadcast_Code: ");
                for (index = 0U; index < GA_BROADCAST_CODE_SIZE; index++)
                {
                    APPL_TRC("0%x ", bc_code[index]);
                }
                APPL_TRC("\n");
                bc_code_ptr = &bc_code[0U];
            }
            printf ("Enable Broadcast Sink...\n");
            retval = GA_broadcast_enable
                     (
                         appl_bcast_synchandle,
                         bc_code_ptr,
                         appl_bcast_bis_count,
                         appl_bcast_bis
                     );
            printf ("Retval - 0x%04X\n", retval);
            break;

        case 95:
            printf ("Disable Broadcast Sink...\n");
            retval = GA_broadcast_disable
                     (
                         appl_bcast_enable_synchandle
                     );
            printf ("Retval - 0x%04X\n", retval);
            break;
#endif /* BAP_BROADCAST */

        case 100:
            if (APPL_GA_INIT_STATUS_LEGACY == ga_init_status)
            {
                printf("GA Shutdown...\n");
                retval = GA_shutdown();
                printf("Retval - 0x%04X\n", retval);
                if (GA_SUCCESS == retval)
                {
                    ga_init_status = APPL_GA_INIT_STATUS_NONE;
                }
            }
            else
            {
                APPL_ERR("Invalid status for GA_shutdown() %d!\n",
                ga_init_status);
            }
            break;

        case 101:
#ifdef BAP_SUPPORT_CONFIG_SELECTION
            printf("GA_update_ntf_configuration...\n");
            retval = GA_update_ntf_configuration
                     (
                         (BAP_CHAR_CONFIG(BAP_CHAR_ID_SOURCE_PAC) |
                          BAP_CHAR_CONFIG(BAP_CHAR_ID_SINK_PAC) |
                          BAP_CHAR_CONFIG(BAP_CHAR_ID_AVAILABLE_AUDIO_CONTEXTS) |
                          BAP_CHAR_CONFIG(BAP_CHAR_ID_SUPPORTED_AUDIO_CONTEXTS)  |
                          BAP_CHAR_CONFIG(BAP_CHAR_ID_ASE_CP) |
                          BAP_CHAR_CONFIG(BAP_CHAR_ID_ASE_SOURCE)  |
                          BAP_CHAR_CONFIG(BAP_CHAR_ID_ASE_SINK )
                         )
                     );
            printf("Retval - 0x%04X\n", retval);
#else /* BAP_SUPPORT_CONFIG_SELECTION */
            printf("BAP_SUPPORT_CONFIG_SELECTION NOT defined\n");
#endif /* BAP_SUPPORT_CONFIG_SELECTION */
            break;

        case 105:
#ifdef BAP_SUPPORT_CONTEXT_MANAGE
            retval = GA_get_context_info
                     (
                         &appl_ga_device,
                         &appl_ga_context,
                         &appl_ga_svc_range_ptr,
                         &appl_ga_char_info_ptr,
                         &appl_ga_char_count
                     );
            printf("Retval - 0x%04X\n", retval);

            if (GA_SUCCESS == retval)
            {
                GA_mem_copy
                (
                    appl_ga_svc_range,
                    appl_ga_svc_range_ptr,
                    (BAP_SERVICE_COUNT * sizeof(GA_BRR_SVC_RANGE))
                );

                /**
                 *  Reference to allocate to save locally.
                 *  This can be written to persistent and retrieved
                 *  upon the next connection if bonded device
                 */
                appl_ga_char_info = GA_alloc_mem(appl_ga_char_count * sizeof(GA_CHAR_INFO));
                if (NULL != appl_ga_char_info)
                {
                    GA_mem_copy(appl_ga_char_info, appl_ga_char_info_ptr, (appl_ga_char_count * sizeof(GA_CHAR_INFO)));
                }
            }
#else /* BAP_SUPPORT_CONTEXT_MANAGE */
            printf ("BAP_SUPPORT_CONTEXT_MANAGE NOT defined\n");
#endif /* BAP_SUPPORT_CONTEXT_MANAGE */
            break;

        case 106:
#ifdef BAP_SUPPORT_CONTEXT_MANAGE
            appl_ga_svc_range_ptr = appl_ga_svc_range;
            appl_ga_char_info_ptr = appl_ga_char_info;

            retval = GA_set_context_info
                     (
                         &appl_ga_device,
                         &appl_ga_context,
                         &appl_ga_svc_range_ptr,
                         &appl_ga_char_info_ptr,
                         &appl_ga_char_count
                     );
            printf("Retval - 0x%04X\n", retval);

            if (GA_SUCCESS == retval)
            {
                if (NULL != appl_ga_char_info)
                {
                    GA_free_mem(appl_ga_char_info);
                    appl_ga_char_info = NULL;
                }
            }
#else /* BAP_SUPPORT_CONTEXT_MANAGE */
            printf ("BAP_SUPPORT_CONTEXT_MANAGE NOT defined\n");
#endif /* BAP_SUPPORT_CONTEXT_MANAGE */
            break;

        case 110:
            appl_ga_automate_unicast_server_operations();
            break;

        case 111:
            appl_ga_automate_unicast_client_operations();
            break;

        default:
            printf("Invalid Choice\n");
            break;
        }

        if (0 == menu_choice)
        {
            /* return */
            break;
        }
    }

    return 0;
}

#ifdef BAP_BROADCAST
void appl_ga_prep_bcast_for_streaming(UINT8 direction, UINT16 bis_conn_hndl)
{
    UINT8 data_path_direction;
    UINT8 index;
    UINT8 codec_id[5];
    GA_RESULT retval;

#ifdef INCLUDE_ETHERMIND_LC3
    INT32 lc3_encoded_bytes;
#endif  /* INCLUDE_ETHERMIND_LC3 */

#ifdef INCLUDE_ETHERMIND_LC3
    /* Parameters related to LC3 Encoder */
    INT32 target_enc_bytes[APPL_LC3_CHANNELS_MAX];
#endif  /* INCLUDE_ETHERMIND_LC3 */

    /* Parameters related to Audio PL Layer */
    UINT16 appl_ga_audio_pl_src_buff_count;

    appl_ga_audio_pl_src_buff_count = 1536;
    data_path_direction = APPL_GA_ISO_UNKNOWN;
    index = 0;
    retval = GA_FAILURE;

    if (0x01 == direction)
    {
        BT_debug_trace(BT_MODULE_ID_WT, "appl_ga_prep_bcast_for_streaming Inside\n");
        data_path_direction = APPL_GA_ISO_TX;
        BT_debug_trace(BT_MODULE_ID_WT, "appl_ga_prep_bcast_for_streaming APPL_GA_ISO_TX\n");
    }
    else
    {
        BT_debug_trace(BT_MODULE_ID_WT, "appl_ga_prep_bcast_for_streaming Inside\n");
        data_path_direction = APPL_GA_ISO_RX;
        BT_debug_trace(BT_MODULE_ID_WT, "appl_ga_prep_bcast_for_streaming APPL_GA_ISO_RX\n");
    }

    /**
     * If HCI is used when setting up their respective audio data paths,
     * and if the codec in use resides in the Bluetooth Host,
     * Codec_Configuration_Length should be 0 but the Coding_Format
     * should be set to Transparent.
     */
    GA_mem_set(codec_id, 0x00, 5);
    codec_id[0] = 0x03; /* Transparent */

    APPL_TRC("Setup ISO Data Path:\n");
    APPL_TRC("BT_hci_le_setup_iso_data_path data_path_direction %d\n", data_path_direction);
    retval = BT_hci_le_setup_iso_data_path
             (
                 bis_conn_hndl,
                 data_path_direction,
                 0x00, /* HCI */
                 codec_id,
                 APPL_GA_ISO_CONTROLLER_DELAY,
                 0,
                 NULL
             );

    if (API_SUCCESS != retval)
    {
        APPL_TRC("ISO DataPath Set Failed = %04X\n", retval);
    }
    else
    {
        APPL_TRC("ISO DataPath Set Successfully...\n");

        active_iso_handle = bis_conn_hndl;

        if (APPL_GA_ISO_TX == data_path_direction)
        {
            APPL_TRC("Audio Data Path TX Setup\n");
#ifdef APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA
            audio_setup_pl
            (
                AUDIO_EP_SOURCE,
                appl_ga_audio_pl_src_callback,
                APPL_GA_CODEC_SUPP_SAMPL_FREQ,
                APPL_GA_SAMPLE_SIZE_IN_BITS,
                APPL_GA_ASE_CHANNEL_COUNT,
                appl_ga_audio_pl_src_buff_count /* TODO: Doesnt really matter, since audio_pl generated more than this */
            );
#endif /* APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA */

#ifdef INCLUDE_ETHERMIND_LC3
           /**
            * Derive No of Bytes pre and post encoding process.
            *      n_f_len = f_s/100;
            *              = 16000/100 = 160;
            *
            *      if (75 == frame_duration)
            *      {
            *          n_f_len = ((n_f_len * 3) >> 2);
            *                  = (160 * 3) / 2 = 120
            *      }
            *      else
            *      {
            *      }
            *
            *      bitrate = samp freq * bit depth(sample size) * No of channels
            *              = 16000 * 2 * 1 = 32kbps.
            *      As per Spec, its 32kbps.
            *
            *      No of Bytes for Encoding:
            *          (SF(inKHz) * (Interval * 10) / (10 * nofchannels * 2)
            *           16 * (10 * 10) / (10 * 1 * 2) = 80B.
            *
            *      No of Bytes post Encoding:
            *           bitrate * (INT32)n_f_len / (8 * sample_rate);
            *           32000 * (160 / (8 * 16000))
            *           40 Bytes.
            *
            *      Refer LC3_encoder_convert_bitrate_to_bytes()
            */

            GA_mem_set(pcm_buffer, 0, sizeof(pcm_buffer));
            GA_mem_set(enc_buffer, 0, sizeof(enc_buffer));

            bytes_to_lc3_encode =
                ((APPL_GA_CODEC_SUPP_SAMPL_FREQ * APPL_GA_CODEC_SUPP_FRAME_DURATION_MS * APPL_GA_ASE_CHANNEL_COUNT * 2) /
                    (10 * 1000));

            lc3_encoded_bytes =
                                LC3_encoder_convert_bitrate_to_bytes
                                (
                                    APPL_GA_CODEC_BITRATE_BPS,
                                    APPL_GA_CODEC_SUPP_SAMPL_FREQ,
                                    APPL_GA_CODEC_SUPP_FRAME_DURATION_MS
                                );

            GA_mem_set
            (
                target_enc_bytes,
                0x00,
                (APPL_LC3_CHANNELS_MAX * sizeof(INT32))
            );

            for (index = 0; index < APPL_GA_ASE_CHANNEL_COUNT; index++)
            {
                /*
                 * Assign Appl Buffer for pcm_buf - Source -> PCM Input,
                 *                        enc_buf - Resultant -> LC3 Enc Output
                 */
                pcm_buf[index] = &pcm_buffer[index][0];
                enc_buf[index] = &enc_buffer[index][0];
                target_enc_bytes[index] = lc3_encoded_bytes;
            }

            /* Init LC3 Encode Module */
            if (LC3_ENCODER_SUCCESS == LC3_encoder_create
                                       (
                                           &lc3_enc_ctx,
                                           APPL_GA_CODEC_SUPP_SAMPL_FREQ,
                                           APPL_GA_SAMPLE_SIZE_IN_BITS,
                                           APPL_GA_ASE_CHANNEL_COUNT,
                                           APPL_GA_CODEC_SUPP_FRAME_DURATION_MS,
                                           target_enc_bytes,
                                           enc_core_buffer,
                                           enc_work_buffer,
                                           pcm_buf,
                                           enc_buf
                                       ))
            {
                appl_ga_lc3_enc_ready = GA_TRUE;
            }
#endif /* INCLUDE_ETHERMIND_LC3 */
        }
        else if (APPL_GA_ISO_RX == data_path_direction)
        {
            APPL_TRC("Audio Data Path RX Setup \n");
#ifdef APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA
            /* Setup Module for Media Data - Play Audio Data */
            audio_setup_pl
            (
                AUDIO_EP_SINK,
                appl_ga_audio_pl_snk_callback,
                APPL_GA_CODEC_SUPP_SAMPL_FREQ,
                APPL_GA_SAMPLE_SIZE_IN_BITS,
                APPL_GA_ASE_CHANNEL_COUNT,
                appl_ga_audio_pl_src_buff_count /* TODO: */
            );
#endif /* APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA */

#ifdef INCLUDE_ETHERMIND_LC3
            /* Init LC3 Decode Module */
            GA_mem_set(enc_buffer, 0, sizeof(enc_buffer));
            GA_mem_set(dec_buffer, 0, sizeof(dec_buffer));

           /*
            * Assign Appl Buffer for enc_buf - Source -> LC3 Encoded Input,
            *                        dec_buf - Resultant -> LC3 Decoded Output
            */
            for (index = 0; index < APPL_LC3_CHANNELS_MAX; index++)
            {
                enc_buf[index] = &enc_buffer[index][0];
                dec_buf[index] = &dec_buffer[index][0];
            }

            if (LC3_DECODER_SUCCESS == LC3_decoder_create
                                       (
                                           &lc3_dec_ctx,
                                           APPL_GA_CODEC_SUPP_SAMPL_FREQ,
                                           APPL_GA_SAMPLE_SIZE_IN_BITS,
                                           APPL_GA_ASE_CHANNEL_COUNT,
                                           APPL_GA_CODEC_SUPP_FRAME_DURATION_MS,
                                           APPL_GA_LC3_DEC_PLC_TYPE_DEFAULT,
                                           dec_core_buffer,
                                           dec_work_buffer,
                                           enc_buf,
                                           dec_buf
                                       ))
            {
                appl_ga_lc3_dec_ready = GA_TRUE;
                APPL_TRC("LC3 Decoder Setup Ready\n");
            }
#endif /* INCLUDE_ETHERMIND_LC3 */
        }
    }

    return;
}
#endif /* BAP_BROADCAST */

void appl_ga_prep_for_streaming(UINT8 local_ase_db_index)
{
    UINT8 data_path_direction;
    UINT8 index;
    UINT8 codec_id[5];
    GA_RESULT retval;

#ifdef INCLUDE_ETHERMIND_LC3
    INT32 lc3_encoded_bytes;
#endif  /* INCLUDE_ETHERMIND_LC3 */

#ifdef INCLUDE_ETHERMIND_LC3
    /* Parameters related to LC3 Encoder */
    INT32 target_enc_bytes[APPL_LC3_CHANNELS_MAX];
#endif  /* INCLUDE_ETHERMIND_LC3 */

    /* Parameters related to Audio PL Layer */
    UINT16 appl_ga_audio_pl_src_buff_count;

    appl_ga_audio_pl_src_buff_count = 1536;
    data_path_direction = APPL_GA_ISO_UNKNOWN;
    index = 0;
    retval = GA_FAILURE;

    if (APPL_GA_GATT_SERVER == curr_GATT_role)
    {
        BT_debug_trace(BT_MODULE_ID_WT, "appl_ga_prep_for_streaming Inside 0x%02x\n", local_ase_db_index);
        if (GA_ROLE_SOURCE == ga_local_audio_ase[local_ase_db_index].codec_conf.role)
        {
            data_path_direction = APPL_GA_ISO_TX;
            BT_debug_trace(BT_MODULE_ID_WT, "appl_ga_prep_for_streaming APPL_GA_ISO_TX 0x%02x\n", local_ase_db_index);
        }
        else if (GA_ROLE_SINK == ga_local_audio_ase[local_ase_db_index].codec_conf.role)
        {
            data_path_direction = APPL_GA_ISO_RX;

            BT_debug_trace(BT_MODULE_ID_WT, "appl_ga_prep_for_streaming APPL_GA_ISO_RX 0x%02x\n", local_ase_db_index);

        }
    }
    else
    {
        /* Remote role, So reverse the role */
        if (GA_ROLE_SINK == ga_rem_dev_info[curr_rem_index].ga_rem_ase[local_ase_db_index].ase_role)
        {
            APPL_TRC("Setup ISO Data Path: Tx Path \n");
            data_path_direction = APPL_GA_ISO_TX;
        }
        else if (GA_ROLE_SOURCE == ga_rem_dev_info[curr_rem_index].ga_rem_ase[local_ase_db_index].ase_role)
        {
            APPL_TRC("Setup ISO Data Path: Rx Path \n");
            data_path_direction = APPL_GA_ISO_RX;
        }
    }

    /**
     * If HCI is used when setting up their respective audio data paths,
     * and if the codec in use resides in the Bluetooth Host,
     * Codec_Configuration_Length should be 0 but the Coding_Format
     * should be set to Transparent.
     */
    GA_mem_set(codec_id, 0x00, 5);
    codec_id[0] = 0x03; /* Transparent */

    if (APPL_GA_GATT_SERVER == curr_GATT_role)
    {
        APPL_TRC("Setup ISO Data Path:\n");
        APPL_TRC("BT_hci_le_setup_iso_data_path data_path_direction %d\n", data_path_direction);
        retval = BT_hci_le_setup_iso_data_path
                 (
                     ga_local_audio_ase[local_ase_db_index].cis_conn_handle,
                     data_path_direction,
                     0x00, /* HCI */
                     codec_id,
                     APPL_GA_ISO_CONTROLLER_DELAY,
                     0,
                     NULL
                 );
    }
    else
    {
        APPL_TRC("Setup ISO Data Path:\n");
        APPL_TRC("BT_hci_le_setup_iso_data_path data_path_direction %d\n", data_path_direction);
        retval = BT_hci_le_setup_iso_data_path
                 (
                     ga_rem_dev_info[curr_rem_index].ga_rem_ase[local_ase_db_index].cis_conn_handle,
                     data_path_direction,
                     0x00, /* HCI */
                     codec_id,
                     APPL_GA_ISO_CONTROLLER_DELAY,
                     0,
                     NULL
                 );
    }
    if (API_SUCCESS != retval)
    {
        APPL_TRC("ISO DataPath Set Failed = %04X\n", retval);
    }
    else
    {
        APPL_TRC("ISO DataPath Set Successfully...\n");
        APPL_TRC("appl_ga_prep_for_streaming ISO DataPath Set Successfully 0x%02x\n", local_ase_db_index);

        if (APPL_GA_ISO_TX == data_path_direction)
        {
            APPL_TRC("Audio Data Path TX Setup 0x%02x\n", local_ase_db_index);
#ifdef APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA
            audio_setup_pl
            (
                AUDIO_EP_SOURCE,
                appl_ga_audio_pl_src_callback,
                APPL_GA_CODEC_SUPP_SAMPL_FREQ,
                APPL_GA_SAMPLE_SIZE_IN_BITS,
                APPL_GA_ASE_CHANNEL_COUNT,
                appl_ga_audio_pl_src_buff_count /* TODO: Doesnt really matter, since audio_pl generated more than this */
            );
#endif /* APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA */

#ifdef INCLUDE_ETHERMIND_LC3
           /**
            * Derive No of Bytes pre and post encoding process.
            *      n_f_len = f_s/100;
            *              = 16000/100 = 160;
            *
            *      if (75 == frame_duration)
            *      {
            *          n_f_len = ((n_f_len * 3) >> 2);
            *                  = (160 * 3) / 2 = 120
            *      }
            *      else
            *      {
            *      }
            *
            *      bitrate = samp freq * bit depth(sample size) * No of channels
            *              = 16000 * 2 * 1 = 32kbps.
            *      As per Spec, its 32kbps.
            *
            *      No of Bytes for Encoding:
            *          (SF(inKHz) * (Interval * 10) / (10 * nofchannels * 2)
            *           16 * (10 * 10) / (10 * 1 * 2) = 80B.
            *
            *      No of Bytes post Encoding:
            *           bitrate * (INT32)n_f_len / (8 * sample_rate);
            *           32000 * (160 / (8 * 16000))
            *           40 Bytes.
            *
            *      Refer LC3_encoder_convert_bitrate_to_bytes()
            */

            if (APPL_GA_GATT_SERVER == curr_GATT_role)
            {
                active_iso_handle = ga_local_audio_ase[local_ase_db_index].cis_conn_handle;
                BT_debug_trace(BT_MODULE_ID_WT, "appl_ga_prep_for_streaming active_iso_handle 0x%02x\n", local_ase_db_index);
            }
            else
            {
                active_iso_handle = ga_rem_dev_info[curr_rem_index].ga_rem_ase[local_ase_db_index].cis_conn_handle;
            }

            GA_mem_set(pcm_buffer, 0, sizeof(pcm_buffer));
            GA_mem_set(enc_buffer, 0, sizeof(enc_buffer));

            bytes_to_lc3_encode =
                ((APPL_GA_CODEC_SUPP_SAMPL_FREQ * APPL_GA_CODEC_SUPP_FRAME_DURATION_MS * APPL_GA_ASE_CHANNEL_COUNT * 2) /
                    (10 * 1000));

            lc3_encoded_bytes = LC3_encoder_convert_bitrate_to_bytes
                                (
                                    APPL_GA_CODEC_BITRATE_BPS,
                                    APPL_GA_CODEC_SUPP_SAMPL_FREQ,
                                    APPL_GA_CODEC_SUPP_FRAME_DURATION_MS
                                );

            GA_mem_set
            (
                target_enc_bytes,
                0x00,
                (APPL_LC3_CHANNELS_MAX * sizeof (INT32))
            );

            for (index = 0; index < APPL_GA_ASE_CHANNEL_COUNT; index++)
            {
                /*
                 * Assign Appl Buffer for pcm_buf - Source -> PCM Input,
                 *                        enc_buf - Resultant -> LC3 Enc Output
                 */
                pcm_buf[index] = &pcm_buffer[index][0];
                enc_buf[index] = &enc_buffer[index][0];
                target_enc_bytes[index] = lc3_encoded_bytes;
            }

            /* Init LC3 Encode Module */
            if (LC3_ENCODER_SUCCESS == LC3_encoder_create
                                       (
                                           &lc3_enc_ctx,
                                           APPL_GA_CODEC_SUPP_SAMPL_FREQ,
                                           APPL_GA_SAMPLE_SIZE_IN_BITS,
                                           APPL_GA_ASE_CHANNEL_COUNT,
                                           APPL_GA_CODEC_SUPP_FRAME_DURATION_MS,
                                           target_enc_bytes,
                                           enc_core_buffer,
                                           enc_work_buffer,
                                           pcm_buf,
                                           enc_buf
                                       ))
            {
                appl_ga_lc3_enc_ready = GA_TRUE;
                APPL_TRC("LC3 Encoder Setup Ready 0x%02x\n", local_ase_db_index);

                if (APPL_GA_GATT_SERVER == curr_GATT_role)
                {
                    ga_local_audio_ase[local_ase_db_index].bytes_to_lc3_encode = bytes_to_lc3_encode;
                    ga_local_audio_ase[local_ase_db_index].lc3_encoded_bytes = lc3_encoded_bytes;
                }
                else
                {
                    ga_rem_dev_info[curr_rem_index].ga_rem_ase[local_ase_db_index].bytes_to_lc3_encode =
                        bytes_to_lc3_encode;
                    ga_rem_dev_info[curr_rem_index].ga_rem_ase[local_ase_db_index].lc3_encoded_bytes =
                        lc3_encoded_bytes;
                }
            }
#endif /* INCLUDE_ETHERMIND_LC3 */
        }
        else if (APPL_GA_ISO_RX == data_path_direction)
        {
            APPL_TRC("Audio Data Path RX Setup 0x%02x\n", local_ase_db_index);
#ifdef APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA
            /* Setup Module for Media Data - Play Audio Data */
            audio_setup_pl
            (
                AUDIO_EP_SINK,
                appl_ga_audio_pl_snk_callback,
                APPL_GA_CODEC_SUPP_SAMPL_FREQ,
                APPL_GA_SAMPLE_SIZE_IN_BITS,
                APPL_GA_ASE_CHANNEL_COUNT,
                appl_ga_audio_pl_src_buff_count /* TODO: */
            );
#endif /* APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA */

#ifdef INCLUDE_ETHERMIND_LC3
            /* Init LC3 Decode Module */
            GA_mem_set(enc_buffer, 0, sizeof(enc_buffer));
            GA_mem_set(dec_buffer, 0, sizeof(dec_buffer));

           /*
            * Assign Appl Buffer for enc_buf - Source -> LC3 Encoded Input,
            *                        dec_buf - Resultant -> LC3 Decoded Output
            */
            for (index = 0; index < APPL_LC3_CHANNELS_MAX; index++)
            {
                enc_buf[index] = &enc_buffer[index][0];
                dec_buf[index] = &dec_buffer[index][0];
            }

            if (LC3_DECODER_SUCCESS == LC3_decoder_create
                                       (
                                           &lc3_dec_ctx,
                                           APPL_GA_CODEC_SUPP_SAMPL_FREQ,
                                           APPL_GA_SAMPLE_SIZE_IN_BITS,
                                           APPL_GA_ASE_CHANNEL_COUNT,
                                           APPL_GA_CODEC_SUPP_FRAME_DURATION_MS,
                                           APPL_GA_LC3_DEC_PLC_TYPE_DEFAULT,
                                           dec_core_buffer,
                                           dec_work_buffer,
                                           enc_buf,
                                           dec_buf
                                       ))
            {
                appl_ga_lc3_dec_ready = GA_TRUE;
                APPL_TRC("LC3 Decoder Setup Ready 0x%02x\n", local_ase_db_index);
            }
#endif /* INCLUDE_ETHERMIND_LC3 */
        }
    }

    return;
}

#ifdef APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA
static void appl_ga_audio_pl_snk_callback
            (
                /* IN */ const UCHAR * p_data,
                /* IN */ UINT16        p_datalen
            )
{
    GA_IGNORE_UNUSED_PARAM(p_data);
    GA_IGNORE_UNUSED_PARAM(p_datalen);

    return;
}

/* Callback that is triggered from Audio Pl callback, Call the Queue Module to stagger data flow */
static void appl_ga_audio_pl_src_callback
            (
                /* IN */ const UCHAR * p_data,
                /* IN */ UINT16        p_datalen
            )
{
    /* APPL_TRC(". "); */
    /* APPL_TRC("%lld ", timestamp); */
    /* APPL_TRC("%d ", p_datalen); */

    if (GA_TRUE == ga_local_audio_src_streaming)
    {
#ifdef DUMP_ISO_DATA_TO_FILE
        if (GA_TRUE == is_dump_enabled)
        {
            if (NULL != lc3_audio_pl_src_callback_dump_fd)
            {
                fwrite(p_data, sizeof(UCHAR), p_datalen, lc3_audio_pl_src_callback_dump_fd);
            }
        }
#endif /* DUMP_ISO_DATA_TO_FILE */

#ifdef INCLUDE_ETHERMIND_LC3
        appl_ga_src_enqueue(p_data, p_datalen);
#endif /* INCLUDE_ETHERMIND_LC3 */
    }
    else
    {
        APPL_TRC("ISO: Data generated at wrong ASE state\n");
    }

    return;
}

GA_RESULT appl_ga_src_enqueue
          (
              /* IN */  const UCHAR * data,
              /* IN */  UINT16        datalen
          )
{
    INT32  n_free;
    UINT32 count;

    BT_thread_mutex_lock(&appl_ga_src_th_mutex);

    /*
     *  Get amount of free buffer space.
     */
    if (appl_ga_src_buffer_wr_ptr >= appl_ga_src_buffer_rd_ptr)
    {
        /*
         *    ----------DDDDDDDDDDDDDDD--------------X
         *    ^         ^              ^             ^
         *    |         |              |             |
         *    0       rd_ptr         wr_ptr         max
         *
         *  Free Space => '-----------'
         */
        n_free = (appl_ga_src_buffer_size - appl_ga_src_buffer_wr_ptr)
            + appl_ga_src_buffer_rd_ptr;
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
        n_free = appl_ga_src_buffer_rd_ptr - appl_ga_src_buffer_wr_ptr;
    }

    /*
     *  Do we have enough space to accomodate new data ?
     *  Buffer Empty Condition: when rd_ptr == wr_ptr
     *  Buffer Full  Condition: when diff(rd_ptr, wr_ptr) == 1
     */
    if ((n_free < 1) || (n_free < (INT32)datalen))
    {
        /* Overflow ! */
        APPL_ERR ("+ %d %d + ", n_free, datalen);
        BT_thread_mutex_unlock(&appl_ga_src_th_mutex);
        return GA_FAILURE;
    }

#ifdef DUMP_ISO_DATA_TO_FILE
    if (GA_TRUE == is_dump_enabled)
    {
        if (NULL != lc3_enc_src_enqueue_fd)
        {
            fwrite(data, sizeof(UCHAR), datalen, lc3_enc_src_enqueue_fd);
        }
    }
#endif /* DUMP_ISO_DATA_TO_FILE */

    /* Store new data into Buffer */
    for (count = 0U; count < datalen; count++)
    {
        appl_ga_src_buffer[appl_ga_src_buffer_wr_ptr] = data[count];
        appl_ga_src_buffer_wr_ptr++;

        if (appl_ga_src_buffer_wr_ptr == appl_ga_src_buffer_size)
        {
            appl_ga_src_buffer_wr_ptr = 0;
        }
    }

    if (APPL_GA_SRC_WR_TH_INIT == appl_ga_src_wr_th_state)
    {
        /* Signal the waiting thread */
        appl_ga_src_wr_th_state = APPL_GA_SRC_WR_TH_PLAYING;
        BT_thread_cond_signal(&appl_ga_src_th_cond);
    }

    BT_thread_mutex_unlock(&appl_ga_src_th_mutex);

    return GA_SUCCESS;
}

DECL_STATIC BT_THREAD_RETURN_TYPE appl_ga_src_write_task(BT_THREAD_ARGS args)
{
    INT32  rd_ptr, index, remaining;
    UINT16 bytes_to_send, buf_index, encode_len;

    GA_IGNORE_UNUSED_PARAM(args);

    buf_index = 0U;

    BT_LOOP_FOREVER()
    {
        BT_thread_mutex_lock(&appl_ga_src_th_mutex);

        if (APPL_GA_SRC_WR_TH_INIT == appl_ga_src_wr_th_state)
        {
            /* Wait for data in buffer */
#ifndef HAVE_CONDWAIT_WO_MUTEX
            BT_thread_cond_wait(&appl_ga_src_th_cond, &appl_ga_src_th_mutex);
#else /* HAVE_CONDWAIT_WO_MUTEX */
            BT_thread_mutex_unlock(&appl_ga_src_th_mutex);
            BT_thread_cond_wait(&appl_ga_src_th_cond, &appl_ga_src_th_mutex);
            BT_thread_mutex_lock(&appl_ga_src_th_mutex);
#endif /* HAVE_CONDWAIT_WO_MUTEX */
        }
        else if (APPL_GA_SRC_WR_TH_STOP == appl_ga_src_wr_th_state)
        {
            /* Exit from task */
            BT_thread_mutex_unlock(&appl_ga_src_th_mutex);
            return BT_THREAD_RETURN_VAL_SUCCESS;
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }

        if (appl_ga_src_buffer_wr_ptr >= appl_ga_src_buffer_rd_ptr)
        {
            /*
             *    ----------DDDDDDDDDDDDDDD--------------X
             *    ^         ^              ^             ^
             *    |         |              |             |
             *    0       rd_ptr         wr_ptr         max
             *
             *  Free Space => '-----------'
             */
            remaining = appl_ga_src_buffer_wr_ptr - appl_ga_src_buffer_rd_ptr;
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
            remaining = appl_ga_src_buffer_size -
                (appl_ga_src_buffer_rd_ptr - appl_ga_src_buffer_wr_ptr);
        }

        /* Do we really have anything to read ? */
        if (remaining < bytes_to_lc3_encode)
        {
#ifndef HAVE_CONDWAIT_WO_MUTEX
            /* Wait for data in buffer */
            appl_ga_src_wr_th_state = APPL_GA_SRC_WR_TH_INIT;
            BT_thread_cond_wait(&appl_ga_src_th_cond, &appl_ga_src_th_mutex);
            BT_thread_mutex_unlock(&appl_ga_src_th_mutex);
#else /* HAVE_CONDWAIT_WO_MUTEX */
            appl_ga_src_wr_th_state = APPL_GA_SRC_WR_TH_INIT;
            BT_thread_mutex_unlock(&appl_ga_src_th_mutex);
            BT_thread_cond_wait(&appl_ga_src_th_cond, &appl_ga_src_th_mutex);
#endif /* HAVE_CONDWAIT_WO_MUTEX */

            /* APPL_INF (bt_debug_fd, "remaining < 1\n"); */
            continue;
        }

        buf_index = 0U;

        rd_ptr = appl_ga_src_buffer_rd_ptr;

        bytes_to_send = (remaining >= bytes_to_lc3_encode) ?
            bytes_to_lc3_encode : (UINT16)remaining;

        for (index = 0; index < bytes_to_send; index++)
        {
            appl_ga_src_pcm_to_send[index] = appl_ga_src_buffer[rd_ptr];

            rd_ptr = rd_ptr + 1;
            if (rd_ptr == appl_ga_src_buffer_size)
            {
                rd_ptr = 0;
            }
        }

        /* Update the read pointer */
        appl_ga_src_buffer_rd_ptr = rd_ptr;

        BT_thread_mutex_unlock(&appl_ga_src_th_mutex);

        while (buf_index < bytes_to_send)
        {
            if ((bytes_to_send - buf_index) <= bytes_to_lc3_encode)
            {
                encode_len = bytes_to_send - buf_index;
            }
            else
            {
                encode_len = bytes_to_lc3_encode;
            }

#ifdef INCLUDE_ETHERMIND_LC3
            lc3_encode_n_send
            (
                &appl_ga_src_pcm_to_send[buf_index],
                encode_len
            );
#endif /* INCLUDE_ETHERMIND_LC3 */

            buf_index += encode_len;
        }
    }
}

GA_RESULT lc3_encode_n_send
          (
              UCHAR  * pcm_data,
              UINT16   pcm_datalen
          )
{
    GA_RESULT retval;
    INT32     lc3_encoded_bytes;
    UINT16    index;
    UINT16    seq_num;
    UCHAR     ts_flag;
    UINT32    ts;

    /* Initialize */
    retval = GA_FAILURE;
    lc3_encoded_bytes = 0;
    index = 0;
    seq_num = 0;
    ts_flag = 0;
    ts = 0x000000;

#ifdef INCLUDE_ETHERMIND_LC3

    /**
     * Copy the local buffer contents here to the LC3 Module buffer.
     * Assuming that the channel is 1, hence pass only for 1 channel.
     * Convert UCHAR LE16 PCM Data to UINT16 LE16
     */
    for (index = 0; index < (pcm_datalen >> 1); index++)
    {
        pcm_buffer[0][index] =
            (INT16)((pcm_data[2 * index + 1] << 8) | (pcm_data[2 * index]));
    }
#endif /* INCLUDE_ETHERMIND_LC3 */

#ifdef DUMP_ISO_DATA_TO_FILE
    if (GA_TRUE == is_dump_enabled)
    {
        if (NULL != lc3_enc_pre_media_dump_fd)
        {
            fwrite(pcm_data, sizeof(UCHAR), pcm_datalen, lc3_enc_pre_media_dump_fd);
        }
    }
#endif /* DUMP_ISO_DATA_TO_FILE */

    /* APPL_TRC("ISO: LC3 Module Bytes for encoding: %d \n", pcm_datalen); */

#ifdef INCLUDE_ETHERMIND_LC3
    if (GA_TRUE == appl_ga_lc3_enc_ready)
    {
        lc3_encoded_bytes = LC3_encoder_process(&lc3_enc_ctx);
        /* APPL_TRC("ISO: LC3 Module Encoded Bytes: %d \n", lc3_encoded_bytes); */

        if (lc3_encoded_bytes > 0)
        {

#ifdef DUMP_ISO_DATA_TO_FILE
            if (GA_TRUE == is_dump_enabled)
            {
                if (NULL != lc3_enc_media_dump_fd)
                {
                    fwrite(enc_buf[0], sizeof(UCHAR), lc3_encoded_bytes, lc3_enc_media_dump_fd);
                }
            }
#endif /* DUMP_ISO_DATA_TO_FILE */

            seq_num++;
            retval = BT_hci_iso_write
                     (
                         active_iso_handle,
                         ts_flag,
                         ts,
                         (UINT16)seq_num,
                         enc_buf[0],
                         (UINT16)lc3_encoded_bytes
                     );

            /* APPL_TRC("%lld ", timestamp); */
            if (GA_SUCCESS != retval)
            {
                APPL_TRC("! ");
                 /* APPL_TRC("ISO Data Write: FAILED !!! Error code = %04X\n", retval); */
            }
            else
            {
                APPL_TRC("* ");
                /* Increment the Sequence number here, Roll over happens since it is UINT16 */
                seq_num++;
            }
        }
    }
#endif /* INCLUDE_ETHERMIND_LC3 */

    return GA_SUCCESS;
}
#endif /* APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA */

GA_RESULT appl_ga_fetch_local_ase_db_index_from_cig_cis_id(UINT16 acl_conn_handle, UINT8 cig_id, UINT8 cis_id, UINT8 *ase_db_id, UINT8 *ase_count)
{
    UINT8 index;
    GA_RESULT retval;

    GA_IGNORE_UNUSED_PARAM(acl_conn_handle);

    retval = GA_SUCCESS;

    for (index = 0; index < APPL_GA_MAX_ASE; index++)
    {
        /* TODO: Fix this acl_conn_handle */
        if ((1) /* (ga_local_audio_ase[index].acl_conn_handle == acl_conn_handle) */
            &&
            (ga_local_audio_ase[index].qos_conf.channel.cig_id == cig_id)
            &&
           (ga_local_audio_ase[index].qos_conf.channel.cis_id == cis_id))
        {
            ase_db_id[*ase_count] = index;
            (*ase_count)++;
            APPL_TRC("ase_count: 0x%02x", *ase_count);
        }
    }

    if (0 == *ase_count)
    {
        retval = GA_FAILURE;
    }

    return retval;
}

GA_RESULT appl_ga_fetch_local_ase_db_index_from_acl_conn_handle
          (
              UINT16 acl_conn_handle,
              UINT8* ase_db_id,
              UINT8* ase_count
          )
{
    UINT8 index;
    GA_RESULT retval;

    retval = GA_SUCCESS;

    for (index = 0; index < APPL_GA_MAX_ASE; index++)
    {
        if (ga_local_audio_ase[index].acl_conn_handle == acl_conn_handle)
        {
            ase_db_id[*ase_count] = index;
            (*ase_count)++;
        }
    }

    if (0 == *ase_count)
    {
        retval = GA_FAILURE;
    }

    return retval;
}

GA_RESULT appl_ga_fetch_local_ase_db_index_from_cis_conn_handle(UINT16 cis_conn_handle, UINT8* ase_db_id, UINT8* ase_count)
{
    UINT8 index;
    GA_RESULT retval;

    retval = GA_SUCCESS;

    for (index = 0; index < APPL_GA_MAX_ASE; index++)
    {
        if (ga_local_audio_ase[index].cis_conn_handle == cis_conn_handle)
        {
            ase_db_id[*ase_count] = index;
            (*ase_count)++;
        }
    }

    if (0 == *ase_count)
    {
        retval = GA_FAILURE;
    }

    return retval;
}

GA_RESULT appl_ga_fetch_rem_ase_db_index_from_cig_id(UINT8 cig_id, UINT8* ase_db_id)
{
    UINT8 index;
    GA_RESULT retval;

    retval = GA_SUCCESS;

    for (index = 0; index < APPL_GA_MAX_ASE; index++)
    {
        if ((ga_rem_dev_info[curr_rem_index].ga_rem_ase[index].qos_conf.channel.cig_id == cig_id)
            &&
            (0xFFFF == ga_rem_dev_info[curr_rem_index].ga_rem_ase[index].cis_conn_handle))
        {
            *ase_db_id = index;
            break;
        }
    }

    if (APPL_GA_MAX_ASE == index)
    {
        retval = GA_FAILURE;
    }

    return retval;
}

GA_RESULT appl_ga_fetch_rem_ase_db_index_from_acl_conn_handle
          (
              UINT16 acl_conn_handle,
              UINT8* ase_db_id
          )
{
    UINT8 index;
    GA_RESULT retval;

    retval = GA_SUCCESS;

    for (index = 0; index < APPL_GA_MAX_DEVICES; index++)
    {
        if (ga_rem_dev_info[curr_rem_index].acl_conn_handle == acl_conn_handle)
        {
            *ase_db_id = index;
            break;
        }
    }

    if (APPL_GA_MAX_DEVICES == index)
    {
        retval = GA_FAILURE;
    }

    return retval;
}

GA_RESULT appl_ga_fetch_rem_ase_db_index_from_cis_conn_handle(UINT16 cis_conn_handle, UINT8* ase_db_id, UINT8* ase_count)
{
    UINT8 index;
    GA_RESULT retval;

    retval = GA_SUCCESS;

    for (index = 0; index < APPL_GA_MAX_ASE; index++)
    {
        if (ga_rem_dev_info[curr_rem_index].ga_rem_ase[index].cis_conn_handle == cis_conn_handle)
        {
            ase_db_id[*ase_count] = index;
            (*ase_count)++;
        }
    }

    if (0 == *ase_count)
    {
        retval = GA_FAILURE;
    }

    return retval;
}

GA_RESULT appl_ga_fetch_local_ase_db_index_from_ase_index(UINT8 ase_id, UINT8* ase_db_id)
{
    UINT8 index;
    GA_RESULT retval;

    retval = GA_SUCCESS;

    for (index = 0; index < APPL_GA_MAX_ASE; index++)
    {
        if (ga_local_audio_ase[index].ase_id == ase_id)
        {
            *ase_db_id = index;
            break;
        }
    }

    if (APPL_GA_MAX_ASE == index)
    {
        retval = GA_FAILURE;
    }

    return retval;
}

GA_RESULT appl_ga_fetch_rem_ase_db_index_from_ase_id(UINT8 ase_id, UINT8* ase_db_id)
{
    UINT8 index;
    GA_RESULT retval;

    retval = GA_SUCCESS;

    for (index = 0; index < APPL_GA_MAX_ASE; index++)
    {
        if (ga_rem_dev_info[curr_rem_index].ga_rem_ase[index].ase_id == ase_id)
        {
            *ase_db_id = index;
            break;
        }
    }

    if (APPL_GA_MAX_ASE == index)
    {
        retval = GA_FAILURE;
    }

    return retval;
}

GA_RESULT appl_ga_fetch_rem_free_ase_db_index(UINT8* ase_db_id)
{
    UINT8 index;
    GA_RESULT retval;

    retval = GA_SUCCESS;

    for (index = 0; index < APPL_GA_MAX_ASE; index++)
    {
        if (ga_rem_dev_info[curr_rem_index].ga_rem_ase[index].ase_id == 0xFF)
        {
            *ase_db_id = index;
            break;
        }
    }

    if (APPL_GA_MAX_ASE == index)
    {
        retval = GA_FAILURE;
    }

    return retval;
}

GA_RESULT appl_ga_fetch_rem_free_index
          (
              UINT8 * rem_dev_index
          )
{
    UINT8 index;
    GA_RESULT retval;

    retval = GA_SUCCESS;

    for (index = 0; index < APPL_GA_MAX_DEVICES; index++)
    {
        if (!GA_IS_NON_ZERO_ENDPOINT_ADDR(ga_rem_dev_info[index].ga_device.bd_addr))
        {
            *rem_dev_index = index;
            break;
        }
    }

    if (APPL_GA_MAX_DEVICES == index)
    {
        retval = GA_FAILURE;
    }

    return retval;
}

GA_RESULT appl_ga_fetch_rem_index_from_ctx
          (
              GA_CONTEXT *ctx,
              UINT8 * rem_dev_index
          )
{
    UINT8 index;
    GA_RESULT retval;

    retval = GA_SUCCESS;

    for (index = 0; index < APPL_GA_MAX_DEVICES; index++)
    {
        if (ga_rem_dev_info[index].ga_context == *ctx)
        {
            *rem_dev_index = index;
            break;
        }
    }

    if (APPL_GA_MAX_DEVICES == index)
    {
        retval = GA_FAILURE;
    }

    return retval;
}
void appl_ga_create_codec_spec_cap_ltv
     (
         /* IN */   GA_LC3_CODEC_SPECIFIC_CAP * codec_specific_cap,
         /* OUT */  UCHAR * ltvarray,
         /* OUT */  UINT8 * ltvarray_len
     )
{
    UINT8 marker;

    marker = 0;

    /* Pack the Supported Sampling Frequency */
    ltvarray[marker++] = GA_LTV_CODEC_SPEC_CAP_SSF_LEN;
    ltvarray[marker++] = GA_LTV_CODEC_SPEC_CAP_SSF_TYPE;
    GA_PACK_LE_2_BYTE_VAL
    (
        &ltvarray[marker],
        codec_specific_cap->supported_sf
    );
    marker += 2;

    /* Pack the Supported Frame Duration */
    ltvarray[marker++] = GA_LTV_CODEC_SPEC_CAP_SFD_LEN;
    ltvarray[marker++] = GA_LTV_CODEC_SPEC_CAP_SFD_TYPE;
    GA_PACK_LE_1_BYTE_VAL(&ltvarray[marker], codec_specific_cap->supported_fd);
    marker += 1;

    /* Pack the Supported Audio Channel Counts */
    ltvarray[marker++] = GA_LTV_CODEC_SPEC_CAP_SACC_LEN;
    ltvarray[marker++] = GA_LTV_CODEC_SPEC_CAP_SACC_TYPE;
    GA_PACK_LE_1_BYTE_VAL
    (
        &ltvarray[marker],
        codec_specific_cap->supported_acc
    );
    marker += 1;

    /* Pack the Supported Octets per Codec Frame */
    ltvarray[marker++] = GA_LTV_CODEC_SPEC_CAP_SOPCF_LEN;
    ltvarray[marker++] = GA_LTV_CODEC_SPEC_CAP_SOPCF_TYPE;
    GA_PACK_LE_4_BYTE_VAL
    (
        &ltvarray[marker],
        codec_specific_cap->supported_opcf
    );
    marker += 4;

    /* Pack the Supported Maximum Codec Frames per SDU */
    ltvarray[marker++] = GA_LTV_CODEC_SPEC_CAP_SMCFPSDU_LEN;
    ltvarray[marker++] = GA_LTV_CODEC_SPEC_CAP_SMCFPSDU_TYPE;
    GA_PACK_LE_1_BYTE_VAL
    (
        &ltvarray[marker],
        codec_specific_cap->supported_mcfpSDU
    );
    marker += 1;

    *ltvarray_len = marker;

    return;
}

void appl_ga_create_codec_spec_conf_ltv
     (
         /* IN */  GA_LC3_CODEC_SPECIFIC_CONF * codec_specific_conf,
         /* OUT */ UCHAR * ltvarray,
         /* OUT */ UINT8 * ltvarray_len
     )
{
    UINT8 marker;

    marker = 0;

    /* Pack the Sampling Frequency */
    ltvarray[marker++] = GA_LTV_CODEC_SPEC_CONF_SF_LEN;
    ltvarray[marker++] = GA_LTV_CODEC_SPEC_CONF_SF_TYPE;
    GA_PACK_LE_1_BYTE_VAL(&ltvarray[marker], codec_specific_conf->sf);
    marker += 1;

    /* Pack the Frame Duration */
    ltvarray[marker++] = GA_LTV_CODEC_SPEC_CONF_FD_LEN;
    ltvarray[marker++] = GA_LTV_CODEC_SPEC_CONF_FD_TYPE;
    GA_PACK_LE_1_BYTE_VAL(&ltvarray[marker], codec_specific_conf->fd);
    marker += 1;

    /* Pack the Audio Channel Allocation */
    ltvarray[marker++] = GA_LTV_CODEC_SPEC_CONF_ACA_LEN;
    ltvarray[marker++] = GA_LTV_CODEC_SPEC_CONF_ACA_TYPE;
    GA_PACK_LE_4_BYTE_VAL(&ltvarray[marker], codec_specific_conf->aca);
    marker += 4;

    /* Pack the Octets per Codec Frame */
    ltvarray[marker++] = GA_LTV_CODEC_SPEC_CONF_OPCF_LEN;
    ltvarray[marker++] = GA_LTV_CODEC_SPEC_CONF_OPCF_TYPE;
    GA_PACK_LE_2_BYTE_VAL(&ltvarray[marker], codec_specific_conf->opcf);
    marker += 2;

    /* Pack the Maximum Codec Frames per SDU */
    ltvarray[marker++] = GA_LTV_CODEC_SPEC_CONF_CFBPSDU_LEN;
    ltvarray[marker++] = GA_LTV_CODEC_SPEC_CONF_CFBPSDU_TYPE;
    GA_PACK_LE_1_BYTE_VAL(&ltvarray[marker], codec_specific_conf->mcfpSDU);
    marker += 1;

    *ltvarray_len = marker;

    return;
}

#endif /* APPL_GA_LEGACY_SUPPORT */


void appl_ga_register(void)
{
    GA_RESULT retval;
    UINT8 index, j;
    GA_LC3_CODEC_SPECIFIC_CONF cs_conf;

#ifdef GA_HAVE_DYNAMIC_CONFIG
    GA_CONFIG    appl_ga_config;
#endif /* GA_HAVE_DYNAMIC_CONFIG */

    /* Register with the bearer interface */
    ga_brr_register();

    /* Register with HCI Appl for HCI Events */
    appl_hci_register_callback(&appl_ga_hci_event_callback);

#ifdef GA_HAVE_DYNAMIC_CONFIG
    printf("Configuring GA\n");
    GA_INIT_CONFIG(appl_ga_config);
    GA_configure(&appl_ga_config);
#endif /* GA_HAVE_DYNAMIC_CONFIG */

    printf("Initializing GA\n");
    /* Initialize Unicast core */
    (GA_IGNORE_RETURN_VALUE)GA_uc_client_init(appl_bap_unicast_client_callback);
    (GA_IGNORE_RETURN_VALUE)GA_uc_server_init(appl_bap_unicast_server_callback);

    /* Initialize Broadcast core */
    (GA_IGNORE_RETURN_VALUE)GA_bc_source_init(appl_bap_broadcast_source_callback);
    (GA_IGNORE_RETURN_VALUE)GA_bc_sink_init(appl_bap_broadcast_sink_callback);

    /* Initialize Volume Control core */
    (GA_IGNORE_RETURN_VALUE)GA_vc_client_init(appl_vcp_client_callback);
    (GA_IGNORE_RETURN_VALUE)GA_vc_server_init(appl_vcp_server_callback);

    /* Initialize Microphone Control core */
    (GA_IGNORE_RETURN_VALUE)GA_mc_client_init(appl_micp_client_callback);
    (GA_IGNORE_RETURN_VALUE)GA_mc_server_init(appl_micp_server_callback);

    /* Initialize Coordinated Set core */
    (GA_IGNORE_RETURN_VALUE)GA_cs_client_init(appl_csip_client_callback);
    (GA_IGNORE_RETURN_VALUE)GA_cs_server_init(appl_csip_server_callback);

    retval = GA_init();
    printf("Retval - 0x%04X\n", retval);
    if (GA_SUCCESS == retval)
    {
        ga_init_status = APPL_GA_INIT_STATUS_LEGACY;

        /* ISOC Host Feature Set and register data callback */
        retval = BT_hci_le_set_host_feature(0x20, 0x01);
        printf("Set ISOC Host feature support - 0x%04X\n", retval);

        retval = BT_hci_register_iso_data_handler
                (
                    &appl_ga_hci_iso_data_ind_cb
                );

        if (API_SUCCESS != retval)
        {
            APPL_TRC("FAILED !!! Error code = %04X\n", retval);
        }
        else
        {
            APPL_TRC("ISO Data Callback registered\n");
        }

        /* By default, Set the Role as Client */
        curr_GATT_role = APPL_GA_GATT_CLIENT;

        for (index = 0; index < APPL_GA_MAX_ASE; index++)
        {
            GA_mem_set(&ga_local_audio_ase[index], 0x00, sizeof(APPL_GA_ASE));
            ga_local_audio_ase[index].cis_conn_handle = 0xFFFF;
            ga_local_audio_ase[index].acl_conn_handle = 0xFFFF;
            ga_local_audio_ase[index].ase_id = 0xFF;
            ga_local_audio_ase[index].curr_state = APPL_GA_STATE_REGISTERED;
        }


        for (index = 0; index < APPL_GA_MAX_DEVICES; index++)
        {
            GA_mem_set(&ga_rem_dev_info[index], 0x00, sizeof(APPL_GA_ASE));

            for (j = 0; j < APPL_GA_MAX_ASE; j++)
            {
                GA_mem_set(&ga_local_audio_ase[index], 0x00, sizeof(APPL_GA_ASE));
                ga_rem_dev_info[index].ga_rem_ase[j].ase_id = 0xFF;
                ga_rem_dev_info[index].ga_rem_ase[j].cis_conn_handle = 0xFFFF;
                ga_rem_dev_info[index].ga_rem_ase[j].acl_conn_handle = 0xFFFF;
                ga_rem_dev_info[index].ga_rem_ase[j].curr_state = APPL_GA_STATE_REGISTERED;
            }
        }

        /* Init all the Audio PL related info */
        (void)BT_thread_mutex_init(&appl_ga_src_th_mutex, NULL);
        (void)BT_thread_cond_init(&appl_ga_src_th_cond, NULL);

        /* Initialize the task variables and create the task */
        appl_ga_src_wr_th_state = APPL_GA_SRC_WR_TH_INIT;

        appl_ga_src_buffer_size = APPL_GA_MAX_BUFFER_SIZE;
        appl_ga_src_buffer_rd_ptr = 0U;
        appl_ga_src_buffer_wr_ptr = 0U;

#ifdef APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA
        /* Setup Module for Generate Audio Data and Consume Audio Data */
        audio_init_pl(AUDIO_EP_SOURCE);
        audio_init_pl(AUDIO_EP_SINK);

        /* Create the Write task which will trigger audio produce on audio start */
        audio_create_task_pl(appl_ga_src_write_task);
#endif /* APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA */

        /* Initialize VCP */
        appl_vcp_ct_set_default_values();
        appl_vcp_rd_set_default_values();

        /* Initialize MICP */
        appl_micp_dev_set_default_values();
        appl_micp_ct_set_default_values();

        /* Register VOCS */
        printf("GA_vc_register. for VOCS \n");
        retval = appl_vcp_rd_reg_opt_service
                (
                    VCP_SERVICE_TYPE_VOCS
                );
        printf("Retval - 0x%04X\n", retval);

        /* Register VCP-AICS */
        printf("GA_vc_register..for AICS \n");
        retval = appl_vcp_rd_reg_opt_service
                (
                    VCP_SERVICE_TYPE_AICS
                );
        printf("Retval - 0x%04X\n", retval);

        /* Register MCP-AICS */
        printf("GA_mc_register_aics \n");
        retval = appl_micp_dev_reg_opt_service();
        printf("Retval - 0x%04X\n", retval);

#ifdef BAP_BROADCAST
        /* Initialize broadcast settings */
        appl_bcast_phy = 0x01;
        appl_bcast_packing = 0x00;
        encryption_state = 0x00;
        appl_bcast_stream_codec_ie.length = 0;
        appl_bcast_sdu_interval = 0x002710;
        appl_bcast_max_sdu = 0x00FB;
        appl_bcast_max_latency = 0x0005;
        appl_bcast_rtn = 0x00;
        appl_bcast_framing = 0x00;

        cs_conf.sf = 0x08; /* Value: 48KHz */
        cs_conf.fd = 0x01;  /* Value: 10ms Frame duration */
        cs_conf.aca = 0x03; /* Value: Channel Count 1 - Bit 0  */
        cs_conf.opcf = 0x64;  /* Value: Number of octets supported per codec frame - 100 */
        cs_conf.mcfpSDU = 0x01; /* Value - 1, by default */

        appl_bcast_codecinfo.coding_format = GA_AUDIO_CODING_LC3;
        appl_bcast_codecinfo.company_id = 0;
        appl_bcast_codecinfo.vendor_codec_id = 0;

        GA_mem_set(&appl_bcast_codecinfo.ie, 0, GA_CODEC_IE_LEN);
        appl_ga_create_codec_spec_conf_ltv(&cs_conf, appl_bcast_codecinfo.ie, &appl_bcast_codecinfo.ie_len);

        GA_mem_set(&appl_bcast_metainfo, 0, sizeof(GA_METADATA));
#endif /* BAP_BROADCAST */
    }

    return;
}

void appl_ga_automate_register_pacs(void)
{
    int choice;
    UINT8 role, index;
    GA_RESULT retval;

    /* PACS Record Count */
    UINT8 reg_id[APPL_GA_MAX_AUDIO_CAPABILITIES];
    UINT8 count;

    GA_LC3_CODEC_SPECIFIC_CAP cs_cap;

#ifdef APPL_GA_USE_DEFAULT_VALUES
    GA_IGNORE_UNUSED_PARAM(choice);
#endif /* APPL_GA_USE_DEFAULT_VALUES */

#ifdef APPL_GA_USE_DEFAULT_VALUES
    role = GA_ROLE_SINK;
#else /* APPL_GA_USE_DEFAULT_VALUES */
    printf("Enter Role (1 - Sink, 2 - Source): ");
    scanf("%d", &choice);
    role = (UINT8)choice;
#endif /* APPL_GA_USE_DEFAULT_VALUES */

#ifdef APPL_GA_USE_DEFAULT_VALUES
    count = 1U;
#else /* APPL_GA_USE_DEFAULT_VALUES */
    printf("Enter Capabilities Count: ");
    scanf("%d", &choice);
    count = (UINT8)choice;
#endif /* APPL_GA_USE_DEFAULT_VALUES */

    for (index = 0; index < count; index++)
    {
        if (GA_ROLE_SINK == role)
        {
#ifdef APPL_GA_USE_DEFAULT_VALUES
            ga_local_pacs.supp_sink_contexts = 0x0004;
#else /* APPL_GA_USE_DEFAULT_VALUES */
            /* Adding Sink Capability */
            printf("Enter Supported Contexts (0x):  Prohibited: 0, Media: 4\n");
            scanf("%x", &choice);
            ga_local_pacs.supp_sink_contexts |= (UINT16)choice;
#endif /* APPL_GA_USE_DEFAULT_VALUES */

#ifdef APPL_GA_USE_DEFAULT_VALUES
            ga_local_pacs.avail_sink_contexts = 0x0004;
#else /* APPL_GA_USE_DEFAULT_VALUES */
            printf("Enter Available Contexts (0x):  Prohibited: 0, Media: 4\n");
            scanf("%x", &choice);
            ga_local_pacs.avail_sink_contexts |= (UINT16)choice;
#endif /* APPL_GA_USE_DEFAULT_VALUES */

            ga_local_pacs.sink_audio_locations = 0x00000003; /* Front Left | Front Right */
            ga_local_audio_cap[ga_local_audio_cap_count].pref_audio_contexts = ga_local_pacs.avail_sink_contexts;
        }
        else if (GA_ROLE_SOURCE == role)
        {
#ifdef APPL_GA_USE_DEFAULT_VALUES
            ga_local_pacs.supp_src_contexts = 0x0004;
#else /* APPL_GA_USE_DEFAULT_VALUES */
            /* Adding Source Capability */
            printf("Enter Supported Contexts (0x):  Prohibited: 0, Media: 4\n");
            scanf("%x", &choice);
            ga_local_pacs.supp_src_contexts |= (UINT16)choice;
#endif /* APPL_GA_USE_DEFAULT_VALUES */

#ifdef APPL_GA_USE_DEFAULT_VALUES
            ga_local_pacs.avail_src_contexts = 0x0004;
#else /* APPL_GA_USE_DEFAULT_VALUES */
            printf("Enter Available Contexts (0x):  Prohibited: 0, Media: 4\n");
            scanf("%x", &choice);
            ga_local_pacs.avail_src_contexts |= (UINT16)choice;
#endif /* APPL_GA_USE_DEFAULT_VALUES */

            ga_local_pacs.src_audio_locations = 0x00000003; /* Front Left | Front Right */
            ga_local_audio_cap[ga_local_audio_cap_count].pref_audio_contexts = ga_local_pacs.avail_src_contexts;
        }

        ga_local_audio_cap[ga_local_audio_cap_count].codec.coding_format = GA_AUDIO_CODING_LC3;
        ga_local_audio_cap[ga_local_audio_cap_count].codec.company_id = 0;
        ga_local_audio_cap[ga_local_audio_cap_count].codec.vendor_codec_id = 0;

        GA_mem_set(&ga_local_audio_cap[ga_local_audio_cap_count].codec.ie[0], 0, GA_CODEC_IE_LEN);

        /* APPL_GA_BF_SAMPL_FREQ_16000 | APPL_GA_BF_SAMPL_FREQ_24000 | APPL_GA_BF_SAMPL_FREQ_48000 */

        cs_cap.supported_sf = 0x0080; /* Value: 48KHz - Bit 7 - 0000 0000 1000 0000 */
        cs_cap.supported_fd = 0x02;  /* Value: 10ms Frame duration - Bit 1 */
        cs_cap.supported_acc = 0x01; /* Value: Channel Count 1 - Bit 0  */
        cs_cap.supported_opcf = 0x00640064;  /* Value: Minimum/Maximum number of octets supported per codec frame - 100 */
        cs_cap.supported_mcfpSDU = 0x01; /* Value - 1, by default */

        appl_ga_create_codec_spec_cap_ltv(&cs_cap, ga_local_audio_cap[ga_local_audio_cap_count].codec.ie, &ga_local_audio_cap[ga_local_audio_cap_count].codec.ie_len);
        ga_local_audio_cap_count++;

        printf("[PACS]GA capability ID: 0x%02X\n", ga_local_audio_cap_count);
    }

    retval = GA_register_audio_capabilities
             (
                 role,
                 count,
                 ga_local_audio_cap,
                 reg_id
             );

    printf("[PACS]Registering GA capability: Retval: 0x%04X\n", retval);
    for (index = 0; index < count; index++)
    {
        printf("Registered ID : 0x%02X\n", reg_id[index]);
    }

    if (GA_ROLE_SINK == role)
    {
        retval = GA_register_audio_contexts_and_locations
                 (
                     role,
                     ga_local_pacs.avail_sink_contexts,
                     ga_local_pacs.supp_sink_contexts,
                     ga_local_pacs.sink_audio_locations
                 );
        printf("[PACS]Registering GA Audio Contexts and Locations: 0x%04X\n", retval);
    }
    else if (GA_ROLE_SOURCE == role)
    {
        retval = GA_register_audio_contexts_and_locations
                 (
                     role,
                     ga_local_pacs.avail_src_contexts,
                     ga_local_pacs.supp_src_contexts,
                     ga_local_pacs.src_audio_locations
                 );
        printf("[PACS]Registering GA Audio Contexts and Locations: PACS Record 0x%04X\n", retval);
    }

    if (GA_SUCCESS == retval)
    {
        curr_GATT_role = APPL_GA_GATT_SERVER;
    }

    return;
}

void appl_ga_register_ascs(void)
{
    GA_RESULT retval;
    int choice;
    UINT8 index, ase_count, ase_id, cap_index;

    /* Local Sink QoS Capabilitites */
    GA_QOS_CAP local_pref_qos_cap;
    GA_SEP_CONF local_ase_codec_conf;

    GA_LC3_CODEC_SPECIFIC_CAP cs_cap;
    GA_LC3_CODEC_SPECIFIC_CONF cs_conf;

#ifdef APPL_GA_USE_DEFAULT_VALUES
    GA_IGNORE_UNUSED_PARAM(choice);
#endif /* APPL_GA_USE_DEFAULT_VALUES */

    /* Initialize */
    ase_id = 0U;
    retval = GA_FAILURE;

    /*
     * Minimum 1 ASE for Source and Sink Mandatory since the PACS Char
     * is added by default during Static GATT DB generation.
     */
    printf("[ASCS]Registering ASE\n");

#ifdef APPL_GA_USE_DEFAULT_VALUES
    ase_count = 1U;
#else /* APPL_GA_USE_DEFAULT_VALUES */
    printf("Enter ASE Count: ");
    scanf("%d", &choice);
    ase_count = (UINT8)choice;
#endif /* APPL_GA_USE_DEFAULT_VALUES */

    if (ase_count > GA_MAX_SEP)
    {
        printf("Invalid Num of ASE Entered!!!\n");
    }
    else if (ase_count < 1)
    {
        printf("Register minimum 1 ASE[1 Src or 1 Sink]\n");
    }
    else
    {
        local_pref_qos_cap.pref_framing = GA_FRAMING_UNFRAMED_ISOAL_PDU_SUPP;
        local_pref_qos_cap.pref_phy = GA_PREF_PHY_LE_1M_PHY;
        local_pref_qos_cap.pref_max_transport_latency = APPL_GA_PREF_MAX_TRANS_LATENCY;
        local_pref_qos_cap.pref_presentation_delay_min = APPL_GA_PREF_PRESENTATION_DELAY_MIN;
        local_pref_qos_cap.pref_presentation_delay_max = APPL_GA_PREF_PRESENTATION_DELAY_MAX;
        local_pref_qos_cap.pref_retxn = APPL_GA_PREF_RETXN;
        local_pref_qos_cap.supp_presentation_delay_min = APPL_GA_SUPP_PRESENTATION_DELAY_MIN;
        local_pref_qos_cap.supp_presentation_delay_max = APPL_GA_SUPP_PRESENTATION_DELAY_MAX;

        for (index = 0; index < ase_count; index++)
        {
#ifdef APPL_GA_USE_DEFAULT_VALUES
            local_ase_codec_conf.role = GA_ROLE_SINK;
#else /* APPL_GA_USE_DEFAULT_VALUES */
            printf("Enter ASE Role: (1 - Sink, 2 - Source):");
            scanf("%d", &choice);
            local_ase_codec_conf.role = (UINT8)choice;
#endif /* APPL_GA_USE_DEFAULT_VALUES */

#ifdef APPL_GA_USE_DEFAULT_VALUES
            cap_index = 1U;
#else /* APPL_GA_USE_DEFAULT_VALUES */
            printf("Enter the Codec Capability Index:");
            scanf("%d", &choice);
            cap_index = (UINT8)choice;
#endif /* APPL_GA_USE_DEFAULT_VALUES */

            if ((cap_index > 0) && (cap_index < APPL_GA_MAX_AUDIO_CAPABILITIES))
            {
                cap_index--;
            }
            else
            {
                printf("Capability Index should be >0 and <APPL_GA_MAX_AUDIO_CAPABILITIES\n");
                /* By default, Use first Capability Index */
                cap_index = 0;
                break;
            }

            /* TODO: Remove cs_cap */
            cs_cap.supported_sf = 0x0080; /* Value: 48KHz - Bit 7 - 0000 0000 1000 0000 */
            cs_cap.supported_fd = 0x02;  /* Value: 10ms Frame duration - Bit 1 */
            cs_cap.supported_acc = 0x01; /* Value: Channel Count 1 - Bit 0  */
            cs_cap.supported_opcf = 0x00640064; /* 0x00C800C8; */ /* Value: Minimum/Maximum number of octets supported per codec frame - 100 */
            cs_cap.supported_mcfpSDU = 0x01; /* Value - 1, by default */

            cs_conf.sf = 0x08U;
            cs_conf.fd = 0x01U;
            cs_conf.aca = GA_AUDIO_LOCATION_FL;
            cs_conf.opcf = 100;
            cs_conf.mcfpSDU = 0x01;
            appl_ga_create_codec_spec_conf_ltv(&cs_conf, ga_local_audio_cap[cap_index].codec.ie, &ga_local_audio_cap[cap_index].codec.ie_len);

            local_ase_codec_conf.codec = ga_local_audio_cap[cap_index].codec;

            if (GA_ROLE_SINK == local_ase_codec_conf.role)
            {
                retval = GA_register_audio_sep
                         (
                             local_ase_codec_conf.role,
                             &local_ase_codec_conf.codec,
                             &local_pref_qos_cap,
                             &ase_id
                         );

                printf("[ASCS]Registered ASE ID, Retval - 0x%04X\n", retval);

                if (GA_SUCCESS == retval)
                {
                    printf("Sink ASE ID %d\n", ase_id);
                }
            }
            else if (GA_ROLE_SOURCE == local_ase_codec_conf.role)
            {
                retval = GA_register_audio_sep
                         (
                             local_ase_codec_conf.role,
                             &local_ase_codec_conf.codec,
                             &local_pref_qos_cap,
                             &ase_id
                         );

                printf("[ASCS]Registered ASE ID, Retval - 0x%04X\n", retval);

                if (GA_SUCCESS == retval)
                {
                    printf("Source ASE ID %d\n", ase_id);
                }
            }

            if (GA_SUCCESS == retval)
            {
                ga_local_audio_ase[ga_local_ase_count].ase_id = ase_id;
                GA_mem_copy(&ga_local_audio_ase[ga_local_ase_count].qos_cap, &local_pref_qos_cap, sizeof(GA_QOS_CAP));
                GA_mem_copy(&ga_local_audio_ase[ga_local_ase_count].codec_conf, &local_ase_codec_conf, sizeof(GA_SEP_CONF));
                ga_local_ase_count++;
            }
        }
    }

    return;
}

void appl_ga_register_dynamic_db(void)
{
    GA_RESULT retval;

    retval = GA_FAILURE;

#ifdef GATT_DB_DYNAMIC
    {
        UINT32 appl_ga_signature = 0x1234FFFF;
        retval = ga_brr_dyn_gatt_db_init_pl(&appl_ga_signature);
        printf("GA Setup - 0x%04X\n", retval);
    }
#endif /* GATT_DB_DYNAMIC */

    return;
}

void appl_ga_unicast_set_remote_dev(void)
{
    int choice;

    choice = 0;
#ifdef DUMP_ISO_DATA_TO_FILE
    is_dump_enabled = GA_FALSE;
#endif /* DUMP_ISO_DATA_TO_FILE */

#ifdef APPL_GA_USE_DEFAULT_VALUES
    /* Already Read from the Config File */
    appl_ga_read_config_file();

    if (GA_FALSE == GA_BRR_DEV_ADDR_IS_NON_ZERO(appl_ga_device.bd_addr))
    {
        /* Take Input from the user since it is not in the Config File */
        CONSOLE_OUT("Enter endpoint address: ");
        appl_get_bd_addr(appl_ga_device.bd_addr);
        CONSOLE_OUT("Enter endpoint type: ");
        CONSOLE_IN("%d", &choice);
        appl_ga_device.bd_type = (UINT8)choice;
    }
#else /* APPL_GA_USE_DEFAULT_VALUES */

    CONSOLE_OUT("Enter endpoint address: ");
    appl_get_bd_addr(appl_ga_device.bd_addr);
    CONSOLE_OUT("Enter endpoint type: ");
    CONSOLE_IN("%d", &choice);
    appl_ga_device.bd_type = (UINT8)choice;
#endif /* APPL_GA_USE_DEFAULT_VALUES */

    if (GA_SUCCESS == appl_ga_fetch_rem_free_index(&curr_rem_index))
    {
        printf("Device registered\n");
    }
    else
    {
        printf("Maximum Simultaneous connections reached\n");
    }

    return;
}

void appl_ga_unicast_auth_and_encrypt(void)
{
    API_RESULT retval;
    SMP_AUTH_INFO auth;
    SMP_BD_HANDLE bd_handle;

    auth.pair_mode = SMP_LESC_MODE;
    auth.security = SMP_SEC_LEVEL_1;
    auth.bonding = GA_TRUE;
    auth.ekey_size = 16;

#ifdef SMP_LESC_CROSS_TXP_KEY_GEN
    auth.transport = SMP_LINK_LE;
    auth.xtx_info = SMP_XTX_DISABLE;
    BT_smp_get_link_handle((BT_DEVICE_ADDR*)&appl_ga_device, &bd_handle, auth.transport);
#else /* SMP_LESC_CROSS_TXP_KEY_GEN */
    BT_smp_get_bd_handle((BT_DEVICE_ADDR*)&appl_ga_device, &bd_handle);
#endif /* SMP_LESC_CROSS_TXP_KEY_GEN */

    retval = BT_smp_authenticate
             (
                 &bd_handle,
                 &auth
             );

    CONSOLE_OUT("BT_smp_authenticate, Retval - 0x%04X\n", retval);

    return;
}

void appl_ga_unicast_client_setup(void)
{
    GA_RESULT retval;

#ifdef APPL_GA_USE_DEFAULT_VALUES
    /* Directly uses ga_device */
#else /* APPL_GA_USE_DEFAULT_VALUES */
#if 0
    printf("Enter endpoint address: ");
    appl_get_bd_addr(ga_device.bd_addr);
    printf("Enter endpoint type: ");
    scanf("%d", &choice);
    ga_device.bd_type = (UINT8)choice;
#endif /* 0 */
#endif /* APPL_GA_USE_DEFAULT_VALUES */

    /* Remove reference to old records */
    ga_rem_dev_info[curr_rem_index].ga_rem_audio_cap_count = 0;

    printf("Setup Endpoint\n");
    printf("Setup PACS: Audio Role Discovery - PACS Discovery\n");
    retval = GA_setup(&appl_ga_device, &appl_ga_context);
    printf("Retval - 0x%04X\n", retval);
    printf("Context: %d\n", appl_ga_context);

    if (GA_SUCCESS == retval)
    {
        ga_rem_dev_info[curr_rem_index].ga_context = appl_ga_context;
        ga_rem_dev_info[curr_rem_index].ga_device = appl_ga_device;

        curr_GATT_role = APPL_GA_GATT_CLIENT;
    }

    return;
}

void appl_ga_unicast_client_get_pacs_capabilities(void)
{
    int choice;
    UINT8 role;
    GA_RESULT retval;

#ifdef APPL_GA_USE_DEFAULT_VALUES
    GA_IGNORE_UNUSED_PARAM(choice);
#endif /* APPL_GA_USE_DEFAULT_VALUES */

#ifdef APPL_GA_USE_DEFAULT_VALUES
    role = GA_ROLE_SINK;
#else /* APPL_GA_USE_DEFAULT_VALUES */
    printf("Enter Role (1 - Sink, 2 - Source): ");
    scanf("%d", &choice);
    role = (UINT8)choice;
#endif /* APPL_GA_USE_DEFAULT_VALUES */

    printf("Get PACS Capabilities - Read Audio Capability, Location, Supp/Avail Contexts\n");
    retval = GA_get_capabilities(&ga_rem_dev_info[curr_rem_index].ga_context, role);
    printf("Retval - 0x%04X\n", retval);

    return;
}

void appl_ga_unicast_client_discover_ascs(void)
{
    GA_RESULT retval;

    printf("Discover ASEs in Endpoint - ASCS Discovery\n");
    retval = GA_sep_discover(&ga_rem_dev_info[curr_rem_index].ga_context);
    printf("Retval - 0x%04X\n", retval);

    return;
}

void appl_ga_unicast_client_configure_codec(void)
{
    GA_RESULT retval;
    UINT8 index;
    GA_LC3_CODEC_SPECIFIC_CONF cs_conf;
    UINT8 peer_ase_count, peer_ase_id[2], peer_ase_role[2];
    int choice;
    GA_CODEC_INFO peer_codec_info[2];
    UINT8 target_phy[2];
    UINT8 target_latency[2];

#ifdef APPL_GA_USE_DEFAULT_VALUES
    peer_ase_count = 1U;
#else /* APPL_GA_USE_DEFAULT_VALUES */
    printf("Enter ASE Count: ");
    scanf("%d", &choice);
    peer_ase_count = (UINT8)choice;
#endif /* APPL_GA_USE_DEFAULT_VALUES */
    for (index = 0; index < peer_ase_count; index++)
    {
#ifdef APPL_GA_USE_DEFAULT_VALUES
        peer_ase_id[index] = 1U;
#else /* APPL_GA_USE_DEFAULT_VALUES */
        printf("Enter ASE ID: ");
        scanf("%d", &choice);
        peer_ase_id[index] = (UINT8)choice;
#endif /* APPL_GA_USE_DEFAULT_VALUES */

#ifdef APPL_GA_USE_DEFAULT_VALUES
        peer_ase_role[index] = GA_ROLE_SINK;
#else /* APPL_GA_USE_DEFAULT_VALUES */
        printf("Enter ASE Role: ");
        scanf("%d", &choice);
        peer_ase_role[index] = (UINT8)choice;
#endif /* APPL_GA_USE_DEFAULT_VALUES */

#ifdef APPL_GA_USE_DEFAULT_VALUES
        choice = 1U;
#else /* APPL_GA_USE_DEFAULT_VALUES */
        /* Refer GA_GET_CAPABILITIES_CNF */
        printf("Enter the Capability Set Index: ");
        scanf("%d", &choice);
#endif /* APPL_GA_USE_DEFAULT_VALUES */
        /*
         * The same ga_rem_audio_cap is used during CS Configure,
         * hence change the ie param accordingly from Codec Specific Cap to
         * Codec Specific Config
         */

         /* TODO: From parsed ie, allow the user to choose */

        cs_conf.sf = 0x08; /* Value: 48KHz */
        cs_conf.fd = 0x01;  /* Value: 10ms Frame duration */
        cs_conf.aca = 0x01; /* Value: Channel Count 1 - Bit 0  */
        cs_conf.opcf = 0x64;  /* Value: Number of octets supported per codec frame - 100 */
        cs_conf.mcfpSDU = 0x01; /* Value - 1, by default */

        /*
         * TODO: Chosen a set not defined in BAP Spec,
         * Shall be vendor specific
         */

        /* By default, using BAP Specified Set ID which uses LC3 */
        /*
         * If one of the Set is chosen as defined in BAP Spec,
         * the fields are mandated as below as per Spec.
         */
        appl_ga_utils_bap_set_codec_id_for_lc3_defined_csc_set_id
        (
            &ga_rem_dev_info[curr_rem_index].ga_rem_audio_cap[choice].codec
        );

        GA_mem_set(&ga_rem_dev_info[curr_rem_index].ga_rem_audio_cap[choice].codec.ie[0], 0, GA_CODEC_IE_LEN);
        appl_ga_create_codec_spec_conf_ltv(&cs_conf, ga_rem_dev_info[curr_rem_index].ga_rem_audio_cap[choice].codec.ie, &ga_rem_dev_info[curr_rem_index].ga_rem_audio_cap[choice].codec.ie_len);

        peer_codec_info[index] = ga_rem_dev_info[curr_rem_index].ga_rem_audio_cap[choice].codec;
        target_phy[index] = 2U;
        target_latency[index] = 2U;
    }

    printf("Configure Codec on ASE\n");
    retval = GA_sep_configure
             (
                 &ga_rem_dev_info[curr_rem_index].ga_context,
                 peer_ase_count,
                 peer_ase_id,
                 peer_ase_role,
                 peer_codec_info,
                 target_phy,
                 target_latency
             );
    printf("Retval - 0x%04X\n", retval);

    return;
}

void appl_ga_unicast_client_setup_cig(void)
{
    GA_RESULT retval;
    UINT8 ase_id;
    UCHAR  cig_id;
    UCHAR  cis_count;
    UCHAR  cis_id[APPL_GA_MAX_CIS_COUNT];
    UINT8 index;
    int choice;
    UINT8 rem_ase_db_index;
    UINT16 max_sdu_m_to_s[APPL_GA_MAX_ASE];
    UINT16 max_sdu_s_to_m[APPL_GA_MAX_ASE];
    UINT8 phy_m_to_s[APPL_GA_MAX_ASE];
    UINT8 phy_s_to_m[APPL_GA_MAX_ASE];
    UINT8 retxn_m_to_s[APPL_GA_MAX_ASE];
    UINT8 retxn_s_to_m[APPL_GA_MAX_ASE];

    /* Initialize */
    rem_ase_db_index = 0U;

#ifdef APPL_GA_USE_DEFAULT_VALUES
    GA_IGNORE_UNUSED_PARAM(choice);
    cig_id = 1U;
#else /* APPL_GA_USE_DEFAULT_VALUES */
    printf("Enter CIG ID: ");
    scanf("%d", &choice);
    cig_id = (UINT8)choice;
#endif /* APPL_GA_USE_DEFAULT_VALUES */

#ifdef APPL_GA_USE_DEFAULT_VALUES
    cis_count = 1U;
#else /* APPL_GA_USE_DEFAULT_VALUES */
    printf("Enter CIS Count: ");
    scanf("%d", &choice);
    cis_count = (UINT8)choice;
#endif /* APPL_GA_USE_DEFAULT_VALUES */

    for (index = 0; index < cis_count; index++)
    {
#ifdef APPL_GA_USE_DEFAULT_VALUES
        ase_id = 1U;
#else /* APPL_GA_USE_DEFAULT_VALUES */
        printf("CIS ID to the same device and with the same Conf\n");
        printf("Enter ASE ID: ");
        scanf("%d", &choice);
        ase_id = (UINT8)choice;
#endif /* APPL_GA_USE_DEFAULT_VALUES */

#ifdef APPL_GA_USE_DEFAULT_VALUES
        cis_id[index] = 1U;
#else /* APPL_GA_USE_DEFAULT_VALUES */
        printf("Enter CIS ID: ");
        scanf("%d", &choice);
        cis_id[index] = (UINT8)choice;
#endif /* APPL_GA_USE_DEFAULT_VALUES */

        if (GA_FAILURE == appl_ga_fetch_rem_ase_db_index_from_ase_id(ase_id, &rem_ase_db_index))
        {
            printf("Unknown ASE ID\n");
        }
        else
        {
            /* For below parameters, m_to_s is also assigned the same value as s_to_m */
            ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_db_index].qos_conf.channel.cig_id = cig_id;
            ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_db_index].qos_conf.channel.cis_id = cis_id[index];
            max_sdu_m_to_s[index] = ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_db_index].qos_conf.max_sdu;
            max_sdu_s_to_m[index] = ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_db_index].qos_conf.max_sdu;
            phy_m_to_s[index] = ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_db_index].qos_conf.phy;
            phy_s_to_m[index] = ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_db_index].qos_conf.phy;
            retxn_m_to_s[index] = ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_db_index].qos_conf.retxn;
            retxn_s_to_m[index] = ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_db_index].qos_conf.retxn;
        }
    }

    /* TODO: Multiple device? Multiple Conf? */

    printf("Create CIG Parameters: \n");
    retval = BT_hci_le_set_cig_parameters
             (
                 ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_db_index].qos_conf.channel.cig_id,
                 ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_db_index].qos_conf.sdu_interval,
                 ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_db_index].qos_conf.sdu_interval,
                 ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_db_index].sca,
                 ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_db_index].packing,
                 ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_db_index].qos_conf.framing,
                 ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_db_index].qos_conf.max_transport_latency,
                 ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_db_index].qos_conf.max_transport_latency,
                 cis_count,
                 cis_id,
                 max_sdu_m_to_s,
                 max_sdu_s_to_m,
                 phy_m_to_s,
                 phy_s_to_m,
                 retxn_m_to_s,
                 retxn_s_to_m
             );

    if (API_SUCCESS != retval)
    {
        APPL_TRC("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        APPL_TRC("API returned success...\n");
    }

    return;
}

void appl_ga_unicast_client_configure_qos(void)
{
    GA_RESULT retval;
    UINT8 index;
    int choice;
    UINT8 rem_ase_db_index;
    UINT8 peer_ase_count, peer_ase_id[2];
    GA_QOS_CONF qos_conf[2];

#ifdef APPL_GA_USE_DEFAULT_VALUES
    GA_IGNORE_UNUSED_PARAM(choice);
#endif /* APPL_GA_USE_DEFAULT_VALUES */

#ifdef APPL_GA_USE_DEFAULT_VALUES
    peer_ase_count = 1U;
#else /* APPL_GA_USE_DEFAULT_VALUES */
    printf("Enter ASE Count: ");
    scanf("%d", &choice);
    peer_ase_count = (UINT8)choice;
#endif /* APPL_GA_USE_DEFAULT_VALUES */

    for (index = 0; index < peer_ase_count; index++)
    {
#ifdef APPL_GA_USE_DEFAULT_VALUES
        peer_ase_id[index] = 1U;
#else /* APPL_GA_USE_DEFAULT_VALUES */
        printf("Enter ASE ID: ");
        scanf("%d", &choice);
        peer_ase_id[index] = (UINT8)choice;
#endif /* APPL_GA_USE_DEFAULT_VALUES */

        if (GA_FAILURE == appl_ga_fetch_rem_ase_db_index_from_ase_id(peer_ase_id[index], &rem_ase_db_index))
        {
            printf("Unknown ASE ID\n");
        }
        else
        {
            GA_mem_copy(&qos_conf[index], &ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_db_index].qos_conf, sizeof(GA_QOS_CONF));
        }
    }

    printf("Setup Codec on ASE: QoS Configuration\n");
    retval = GA_sep_setup(&appl_ga_context, peer_ase_count, peer_ase_id, qos_conf);
    printf("Retval - 0x%04X\n", retval);

    return;
}

void appl_ga_unicast_client_enable(void)
{
    GA_RESULT retval;
    int choice;
    UINT8 index, j;
    UINT8 ccid[2];
    UINT8 rem_ase_db_index;
    UINT8 peer_ase_count, peer_ase_id[2];
    GA_CONTENT_INFO content_info[2];

#ifdef APPL_GA_USE_DEFAULT_VALUES
    GA_IGNORE_UNUSED_PARAM(choice);
#endif /* APPL_GA_USE_DEFAULT_VALUES */

#ifdef APPL_GA_USE_DEFAULT_VALUES
    peer_ase_count = 1U;
#else /* APPL_GA_USE_DEFAULT_VALUES */
    printf("Enter ASE Count: ");
    scanf("%d", &choice);
    peer_ase_count = (UINT8)choice;
#endif /* APPL_GA_USE_DEFAULT_VALUES */

    for (index = 0; index < peer_ase_count; index++)
    {
#ifdef APPL_GA_USE_DEFAULT_VALUES
        peer_ase_id[index] = 1U;
#else /* APPL_GA_USE_DEFAULT_VALUES */
        printf("Enter ASE ID: ");
        scanf("%d", &choice);
        peer_ase_id[index] = (UINT8)choice;
#endif /* APPL_GA_USE_DEFAULT_VALUES */

        if (GA_FAILURE == appl_ga_fetch_rem_ase_db_index_from_ase_id(peer_ase_id[index], &rem_ase_db_index))
        {
            printf("Unknown ASE ID\n");
            peer_ase_count--;
        }
        else
        {
#ifdef APPL_GA_USE_DEFAULT_VALUES
            content_info[index].contexts = 0x0004;
#else /* APPL_GA_USE_DEFAULT_VALUES */
            /* TODO: Runtime : BAP Page 95 */
            printf("Enter Context Type: ");
            scanf("%d", &choice);
            content_info[index].contexts = (UINT16)choice;
#endif /* APPL_GA_USE_DEFAULT_VALUES */

            /* TODO: Validate the value to be one amongst the avail_src_contexts and then proceed */

#ifdef APPL_GA_USE_DEFAULT_VALUES
            content_info[index].num_ccid = 1U;
#else /* APPL_GA_USE_DEFAULT_VALUES */
            printf("Enter CCID Count: ");
            scanf("%d", &choice);
            content_info[index].num_ccid = (UINT8)choice;
#endif /* APPL_GA_USE_DEFAULT_VALUES */

            for (j = 0; j < content_info[index].num_ccid; j++)
            {
#ifdef APPL_GA_USE_DEFAULT_VALUES
                ccid[j] = 1U;
#else /* APPL_GA_USE_DEFAULT_VALUES */
                printf("Enter CCID: ");
                scanf("%d", &choice);
                ccid[j] = (UINT8)choice;
#endif /* APPL_GA_USE_DEFAULT_VALUES */
            }
            content_info[index].ccid = ccid;
        }
    }

    printf("Enable ASE\n");
    retval = GA_sep_start(&appl_ga_context, peer_ase_count, peer_ase_id, content_info);
    printf("Retval - 0x%04X\n", retval);

    return;
}

void appl_ga_unicast_client_create_cis(void)
{
    GA_RESULT retval;
    UINT8 index, ase_id;
    UCHAR  cis_count;
    UINT16 cis_connection_handle[APPL_GA_MAX_CIS_COUNT], acl_connection_handle[APPL_GA_MAX_CIS_COUNT];
    UINT8 rem_ase_db_index;
#ifdef APPL_GA_USE_DEFAULT_VALUES
    cis_count = 1U;
#else /* APPL_GA_USE_DEFAULT_VALUES */
    int choice;

    APPL_TRC("Enter cis_count :\n");
    scanf("%x", &choice);
    cis_count = (UCHAR)choice;
#endif /* APPL_GA_USE_DEFAULT_VALUES */

    for (index = 0; index < cis_count; index++)
    {
#ifdef APPL_GA_USE_DEFAULT_VALUES
        ase_id = 1U;
#else /* APPL_GA_USE_DEFAULT_VALUES */
        APPL_TRC("Enter ASE ID :\n");
        scanf("%x", &choice);
        ase_id = (UINT8)choice;
#endif /* APPL_GA_USE_DEFAULT_VALUES */

        if (GA_FAILURE == appl_ga_fetch_rem_ase_db_index_from_ase_id(ase_id, &rem_ase_db_index))
        {
            printf("Unknown ASE ID\n");
        }
        else
        {
            cis_connection_handle[index] = ga_rem_dev_info[curr_rem_index].ga_rem_ase[rem_ase_db_index].cis_conn_handle;
            acl_connection_handle[index] = ga_rem_dev_info[curr_rem_index].acl_conn_handle;
        }
    }

    printf("Create CIS: \n");
    retval = BT_hci_le_create_cis
             (
                 cis_count,
                 cis_connection_handle,
                 acl_connection_handle
             );

    if (API_SUCCESS != retval)
    {
        APPL_TRC("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        APPL_TRC("API returned success...\n");
    }

    return;
}

void appl_ga_read_config_file(void)
{
    CHAR config_buf[GA_CONFIG_MAX_CHARS_PER_LINE];
    INT32 ret;
    int value;

#ifdef DUMP_ISO_DATA_TO_FILE
    /* Read Audio Dump enable from Config File */
    ret = BT_config_read(BT_CONFIG_FILE, "ENABLE_AUDIO_DUMP", config_buf);
    if (ret < 0)
    {
        printf("*** FAILED to Read Configuration for ENABLE_AUDIO_DUMP: %d\n",
            ret);
    }

    printf("Enable Audio Dump = '%s'\n", config_buf);

    if (0 == BT_str_cmp("TRUE", config_buf))
    {
        is_dump_enabled = GA_TRUE;
    }
    else
    {
        is_dump_enabled = GA_FALSE;
    }
#endif /* DUMP_ISO_DATA_TO_FILE */

    GA_mem_set(config_buf, 0x00, sizeof(config_buf));
    if (API_SUCCESS != BT_config_read
                       (
                           "EtherMind.conf",
                           "BD_ADDRESS",
                           (CHAR*)config_buf
                       ))
    {
        APPL_TRC("Failed to read BD_ADDRESS from the Config\n");
    }
    else
    {
        UCHAR* str_temp_ptr = NULL;
        UCHAR* str_ptr = NULL;
        INT32 index;

        APPL_TRC("Peer Device Address\t\t: %s\n", config_buf);

        str_ptr = (UCHAR*)strtok((CHAR*)config_buf, "-");
        str_temp_ptr = str_ptr;

        for (index = 0; index <= 5; index++)
        {
            sscanf((const char*)str_ptr, "%X", &value);
            appl_ga_device.bd_addr[index] = (UCHAR)value;

            if (5 == index)
            {
                break;
            }

            str_ptr = (UCHAR*)strtok(NULL, "-");

            if (NULL == str_ptr)
            {
                str_ptr = str_temp_ptr;
            }
            else
            {
                str_temp_ptr = str_ptr;
            }
        }

        appl_ga_device.bd_type = 0U;
    }

    return;
}

void appl_ga_unicast_server_adv_enable(void)
{
    API_RESULT retval;
    int choice;
    UINT8 advertising_enable;

#ifdef APPL_GA_USE_DEFAULT_VALUES
    GA_IGNORE_UNUSED_PARAM(choice);

    advertising_enable = 1U;
#else /* APPL_GA_USE_DEFAULT_VALUES */
    printf("Enter Advertising Enable (1 - Enable, 0 - Disable): ");
    scanf("%d", &choice);
    advertising_enable = (UINT8)choice;
#endif /* APPL_GA_USE_DEFAULT_VALUES */

    retval = BT_hci_le_set_advertising_enable
             (
                 advertising_enable
             );


    if (API_SUCCESS != retval)
    {
        APPL_TRC("FAILED !!! Error code = %04X\n", retval);
    }
    else
    {
        APPL_TRC("API returned success...\n");
    }

    return;
}

void appl_ga_unicast_client_timer_expiry_handler
     (
         void   * data,
         UINT16   datalen
     )
{
    GA_IGNORE_UNUSED_PARAM(data);
    GA_IGNORE_UNUSED_PARAM(datalen);

    if (BT_TIMER_HANDLE_INIT_VAL != appl_ga_unicast_client_timer_handle)
    {
        appl_ga_unicast_client_timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    }

    appl_ga_unicast_client_setup();

    return;
}

void appl_ga_automate_unicast_server_operations(void)
{
    int choice;

    choice = 0;
#ifdef DUMP_ISO_DATA_TO_FILE
    is_dump_enabled = GA_FALSE;
#endif /* DUMP_ISO_DATA_TO_FILE */

    GA_mem_set(&appl_ga_device, 0x00U, sizeof(GA_ENDPOINT));

    appl_ga_register();
    appl_ga_automate_register_pacs();
    appl_ga_register_ascs();
    appl_ga_register_dynamic_db();
    appl_ga_read_config_file();

    if (GA_FALSE == GA_BRR_DEV_ADDR_IS_NON_ZERO(appl_ga_device.bd_addr))
    {
        /* Take Input from the user since it is not in the Config File */
        CONSOLE_OUT("Enter endpoint address: ");
        appl_get_bd_addr(appl_ga_device.bd_addr);
        CONSOLE_OUT("Enter endpoint type: ");
        CONSOLE_IN("%d", &choice);
        appl_ga_device.bd_type = (UINT8)choice;
    }

    appl_ga_unicast_server_adv_enable();

    return;
}

void appl_ga_automate_unicast_client_operations(void)
{
    appl_ga_register();
    appl_ga_read_config_file();
    appl_ga_unicast_set_remote_dev();
    /* Create Extended Connection with remote */
    appl_hci_le_connect((BT_DEVICE_ADDR*)&appl_ga_device, 0x01U);

    return;
}
#endif /* BT_GAM */
