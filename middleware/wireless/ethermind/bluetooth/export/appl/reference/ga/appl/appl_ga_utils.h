/**
 *  \file appl_ga_utils.h
 *
 *  \brief This file defines the GA profile util methods
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_GA_UTILS_
#define _H_APPL_GA_UTILS_

#include "GA_api.h"
#include "appl_smp.h"
#include "appl_ga_features.h"

#include "ga_brr_pl.h"

#ifdef AUDIO_SRC_LC3_SUPPORT
#include "LC3_api.h"
#endif /* AUDIO_SRC_LC3_SUPPORT */

#define GA_CB_ENTRY(layer) \
        printf("************************** CALLBACK: GA %s **************************\n", (layer));
#define GA_CB_EXIT() \
        printf("*********************************************************************\n");
#define GA_FORMAT_DISPLAY() \
        printf("-----------------------------------------------------------------\n");


#define GA_FORMAT_ADV_DISPLAY() \
        printf("--------------------------------------------------------------------------------------------------------\n"); \
        printf("Length        Type                                     Value\n"); \
        printf("--------------------------------------------------------------------------------------------------------\n");

#define APPL_GA_UNICAST_CLIENT         1 << 1
#define APPL_GA_UNICAST_SERVER         1 << 2

/* Macros for formatting for displaying EA data */
#define GA_EA_ADV_FORMAT_LENGTH_POS    0x0EU
#define GA_EA_ADV_FORMAT_TYPE_POS      0x29U
#define GA_EA_ADV_FORMAT_VALUE_POS     0x37U

#define APPL_GA_INIT_STATUS_NONE       0x00U
#define APPL_GA_INIT_STATUS_LEGACY     0x01U
#define APPL_GA_INIT_STATUS_GA         0x02U

#define APPL_GA_SCAN_INVALID           1 << 0
#define APPL_GA_SCAN_FOR_UCS           1 << 1
#define APPL_GA_SCAN_FOR_SD            1 << 2
#define APPL_GA_SCAN_FOR_BC_SRC        1 << 3

#define APPL_GA_ADV_INVALID            1 << 0
#define APPL_GA_ADV_UCS                1 << 1
#define APPL_GA_ADV_SD                 1 << 2
#define APPL_GA_ADV_BC_SRC             1 << 3

#define GA_CONNECTION_MODE_NORMAL      0x00U
#define GA_CONNECTION_MODE_EXTENDED    0x01U

/* TMAP Roles defined as per GA_tmap_api.h */
/* TMAP Role - None */
#define APPL_GA_ROLE_NONE        (0x0000 << 0U)
/* TMAP Role - Call Gateway (CG) */
#define APPL_GA_TMAP_ROLE_CG     (0x0001 << 0U)
/* TMAP Role - Call Terminal (CT) */
#define APPL_GA_TMAP_ROLE_CT     (0x0001 << 1U)
/* TMAP Role - Unicast Media Sender (UMS) */
#define APPL_GA_TMAP_ROLE_UMS    (0x0001 << 2U)
/* TMAP Role - Unicast Media Receiver (UMR) */
#define APPL_GA_TMAP_ROLE_UMR    (0x0001 << 3U)
/* TMAP Role - Broadcast Media Sender (BMS) */
#define APPL_GA_TMAP_ROLE_BMS    (0x0001 << 4U)
/* TMAP Role - Broadcast Media Receiver (BMR) */
#define APPL_GA_TMAP_ROLE_BMR    (0x0001 << 5U)
/* HAP Role - Hearing Aid (HA) */
#define APPL_GA_HAP_ROLE_HA      (0x0001 << 6U)
/* HAP Role - Hearing Aid Unicast Client (HAUC) */
#define APPL_GA_HAP_ROLE_HAUC    (0x0001 << 7U)
/* HAP Role - Hearing Aid Remote Controller (HARC) */
#define APPL_GA_HAP_ROLE_HARC    (0x0001 << 8U)

/*
 * Maximum Connected Devices List
 */
#define MAX_CONN_DEVICES 5U

/*
 * Maximum Audio Capabilities supported.
 */
#define MAX_SRC_CODEC_CAP   1U
#define MAX_SNK_CODEC_CAP   1U

#define MAX_AUDIO_CAP (MAX_SRC_CODEC_CAP + MAX_SNK_CODEC_CAP)

#define MAX_REM_PACS_CAP_INFO     2U
#define INVALID_CIG_CIS_ID        0xFFU
#define INVALID_CIS_HANDLE        0xFFFFU
#define INVALID_CONN_DEV_INDEX    0xFFU

/* CIG States */
#define CIG_STATE_IDLE            0x00U
#define CIG_STATE_WAITING         0x01U
#define CIG_STATE_CREATED         0x02U

/* CIS States */
#define CIS_STATE_IDLE            0x00U
#define CIS_STATE_INITIALIZED     0x01U
#define CIS_STATE_WAITING         0x03U
#define CIS_STATE_CONNECTED       0x02U

/* ASE related macros */
#define MAX_UCS_SIMULTANEOUS_ASE  2U
#define MAX_UC_ASE                5U
#define ASE_UNIDIRECTIONAL        0x01U
#define ASE_BIDIRECTIONAL         0x02U
#define ASE_DIRECTION_INVALID     0xFFU

/* This depends on the kind of Input File used, move it under AUDIO_PL_SUPPORT? */
#define AUDIO_NUM_OF_BITS_PER_SAMPLE  16U

#if (defined APPL_BAP_BROADCAST_SOURCE) || (defined APPL_BAP_BROADCAST_SINK) || (defined APPL_BAP_BROADCAST_SCAN_DELEGATOR) || (defined APPL_BAP_BROADCAST_ASSISTANT)
#define APPL_BC_MAX_NUM_OF_SUB_GROUPS            5U
#define APPL_BC_MAX_BIS_PER_SUB_GROUP            2U
#endif /* (defined APPL_BAP_BROADCAST_SOURCE) || (defined APPL_BAP_BROADCAST_SINK) || (defined APPL_BAP_BROADCAST_SCAN_DELEGATOR) || (defined APPL_BAP_BROADCAST_ASSISTANT) */

#if ((defined AUDIO_SRC_LC3_SUPPORT) || (defined AUDIO_SNK_LC3_SUPPORT))

/*
 * Ethermind LC3 module requires Frame_Duration in ms in the format below:
 * Frame duration in ms [100 for 10.0ms, 75 for 7.5ms]
 */
#define GET_SUPP_FRAME_DURATION_IN_MS_FOR_LC3(x)\
        ((x) = (x)/(100))

#endif /* ((defined AUDIO_SRC_LC3_SUPPORT) || (defined AUDIO_SNK_LC3_SUPPORT)) */

typedef UINT16 APPL_GA_ROLE;

typedef struct _CONN_DEVICES
{
    /* Connected to Device */
    GA_ENDPOINT    device;
    /* Initiator/Acceptor */
    UINT8          isInitiator;
    GA_CONTEXT     context;
    /* ACL Connection Handle */
    UINT16         conn_handle;
}CONN_DEVICES;

#define APPL_INVALID_CONN_HANDLE 0xFFFFU

typedef struct _PACS_CAP_RECORD
{
    GA_AUDIO_CAP  cap;
    UINT8         cap_record_id;
    GA_LC3_CODEC_SPECIFIC_CAP lc3_cs_cap;
}PACS_CAP_RECORD;

typedef struct _PACS_CAP_INFO
{
    /* PACS DB */
    /* Audio Capabilities - Source PAC */
    PACS_CAP_RECORD       src_cap[MAX_SRC_CODEC_CAP];
    UINT8                 src_cap_count;
    /* Audio Capabilities - Sink PAC */
    PACS_CAP_RECORD       snk_cap[MAX_SNK_CODEC_CAP];
    UINT8                 snk_cap_count;
    /* Sink/Src Audio Locations, Available/Supp Audio Contexts */
    GA_PACS_AUDIO_RECORD  record;
}PACS_CAP_INFO;

typedef struct _REMOTE_PACS_CAP_INFO
{
    PACS_CAP_INFO  pacs;
    UINT8          conn_dev_index;
}REMOTE_PACS_CAP_INFO;

typedef struct _CIG_CIS_INFO
{
    /* CIG State */
    UINT8 cig_state;
    /* CIS Connection handle */
    UINT16 handle;
    /* CIS Connection Status */
    UINT8 cis_state;
}CIG_CIS_INFO;

typedef struct _ASE
{
    /*
     * UC Client:
     * ASE Specific QoS Capability, By default one can choose to give a
     * default QoS Capability. However this can be changed at any point in time
     * by calling GA_update_audio_sep_qos_capabilities().
     * By default some setting is chosen and set during register,
     * but during ASE CP Config Codec, In case the Set requested is different
     * and if App wants it to be changed, then the call can be made before
     * sending Notification.
     * UC Server:
     * During ASE Register, store the capability of the ASE.
     */
    GA_QOS_CAP    qos_cap;

    /*
     *
     */
    GA_LC3_CODEC_SPECIFIC_CONF cs_conf;

    /*
     * UC Client:
     * Once the Config Codec is complete, During setting of CIG Parameters,
     * QoS Conf for each of the ASE is input by the user and configured.
     * UC Server:
     * Once the Config QoS is complete, the configuration is updated for
     * the ASE.
     */
    GA_QOS_CONF   qos_conf;

#ifdef APPL_GA_ALLOW_CODEC_CONF_CACHING
    /* UC Server Only:
     * If Caching is Enabled, then the Codec gets saved during Releasing
     * state of the ASE.
     * Also, the state of the ASE will then move to Codec Configured
     * and the same codec conf will be used during ASE Discovery.
     * Otherwise, the ASE will remain in Idle state and the Codec Configured
     * will not be retained.
     */
    GA_CODEC_INFO* codec;
#endif /* APPL_GA_ALLOW_CODEC_CONF_CACHING */

    /*
     * UC Client & Server: QoS Conf Set ID
     */
    UINT8         qos_conf_set_id;
    /*
     * UC Server:
     * ASE Role - Local.
     * UC Client:
     * ASE Role - Remote.
     */
    UINT8         role;
    /* ASE ID */
    UINT8         id;
    /* ASE State */
    GA_ASE_STATE  state;
    /*
     * UC Server Only:
     */
    /* Connected Device Index */
    UINT8         conn_dev_index[MAX_UCS_SIMULTANEOUS_ASE];
    /*
     * Comment for UC Server Only:
     * TODO: If single ASE is connected to multiple devices, below should be
     * Array. Check if other params also needs to be array.
     */
    /* Info about CIG and CIS */
    CIG_CIS_INFO  cig_cis_info;
    /* Info about ISO State */
    UINT8         iso_state;
    UINT8         is_release_initiated;
}ASE;

typedef struct _ASCS_ASE_INFO
{
    ASE    ase[MAX_UC_ASE];
    UINT8  ase_count;
}ASCS_ASE_INFO;

#ifdef BAP_BROADCAST

typedef struct _APPL_BC_BIS_INFO
{
#if (defined APPL_BAP_BROADCAST_SINK) || (defined APPL_BAP_BROADCAST_SCAN_DELEGATOR)
    /*
     * Sink Side: Fetched during GA_BC_SOURCE_CONFIG_IND, BIS Index
     * Source Side: This field can be ignored since it is added and maintained
     * by the GA Core layer.
     */
    /* BIS Index */
    UINT8       bis_index;
#endif /* (defined APPL_BAP_BROADCAST_SINK) || (defined APPL_BAP_BROADCAST_SCAN_DELEGATOR) */

    /* BIS Handle */
    UINT16      bis_handle;

    /* ISO State */
    UINT8       iso_state;

    /* QOS Conf Set ID */
    UINT8       qos_conf_set_id;

    /* BIS info (Codec_spec_conf) */
    GA_CODEC_IE cs_conf_ltv;

    /* CS Conf */
    GA_LC3_CODEC_SPECIFIC_CONF cs_conf;

    /*
     * cs conf of SG and BIS combined together into one since spec doesn't
     * mandate all fields to be present in both.
     */
    GA_LC3_CODEC_SPECIFIC_CONF cs_conf_sg_bis;

    /* Streaming Status of each of the BIS */
    UINT8       streaming_status;
}APPL_BC_BIS_INFO;

/* This is used for Level - 2 SubGroup Information,
 * used during Broadcast Source Setting
 */
typedef struct _APPL_BC_SUBGROUP
{
    /* Broadcast Streaming Endpoint ID */
    UINT8 bc_sep;

    /* No. of BIS */
    UINT8 bc_nstream;

    /* QOS Conf Set ID */
    UINT8 qos_conf_set_id;

    /* CS Conf */
    GA_LC3_CODEC_SPECIFIC_CONF cs_conf;

    /* SG: Codec_info + Meta_info */
    BAP_BASIC_ANNOUNCEMENT_LEVEL_1 bc_basic_announcement_1;

    /* Information of the BIS */
    APPL_BC_BIS_INFO               bis_info[APPL_BC_MAX_BIS_PER_SUB_GROUP];

} APPL_BC_SUBGROUP;

typedef struct _BC_SINK_PARAMS
{
    /*
     * Below members are used by Broadcast Sink, Scan Delegator and
     * Broadcast Assistant.
     */
    UINT16 bcast_synchandle;
    UINT16 config_synchandle;
    UINT16 stream_synchandle;
    /*
     * NOTE: Below members are used by only Broadcast Sink and
     * Scan Delegator.
     */
#if (defined APPL_BAP_BROADCAST_SINK) || (defined APPL_BAP_BROADCAST_SCAN_DELEGATOR)
    UINT16 enable_synchandle;

    /* BIS Index that will be used for Enable */
    UINT8  bis_count_enable;
    UINT8  bis_index_enable[APPL_BC_MAX_BIS_PER_SUB_GROUP * APPL_BC_MAX_NUM_OF_SUB_GROUPS];
    /* If Multiple BISes are enabled, only one of them can be Active Playback */
    UINT8  bis_index_active_playback;

    UINT8  encryption_state;
    /* Broadcast Code */
    UCHAR bc_code[GA_BROADCAST_CODE_SIZE];

    /* Number of Subgroups */
    UINT8 bc_num_subgroups;

    /*
     * Subgroup Info: No. of BIS + Codec_info + Meta_info
     *                +
     *                Each BIS: BIS info (BIS_index + Codec_spec_conf)
     */
    APPL_BC_SUBGROUP bc_subgroup_info[APPL_BC_MAX_NUM_OF_SUB_GROUPS];
#endif /* (defined APPL_BAP_BROADCAST_SINK) || (defined APPL_BAP_BROADCAST_SCAN_DELEGATOR) */
}BC_SINK_PARAMS;

#if (defined APPL_BAP_BROADCAST_SINK) || (defined APPL_BAP_BROADCAST_SCAN_DELEGATOR) || (defined APPL_BAP_BROADCAST_ASSISTANT)
#define BC_INVALID_HANDLE  0xFFFFU
#endif /* (defined APPL_BAP_BROADCAST_SINK) || (defined APPL_BAP_BROADCAST_SCAN_DELEGATOR) || (defined APPL_BAP_BROADCAST_ASSISTANT) */

#define BC_ROLE_NONE                    (0x00U)
#define BC_ROLE_BA                      (0x01U << 0U)
#define BC_ROLE_BC_SRC                  (0x01U << 1U)
#define BC_ROLE_SD                      (0x01U << 2U)
#define BC_ROLE_BC_SNK                  (0x01U << 3U)

#define SD_GET_ROLE_STRING(role)\
        ((BC_ROLE_SD == role)?("SD\0"):((BC_ROLE_BC_SNK == role)?("BC_SNK\0"):("Unknown\0")))

#define BA_GET_ROLE_STRING(role)\
        ((BC_ROLE_BA == role)?("BA\0"):((BC_ROLE_BC_SRC == role)?("BC_SRC\0"):("BA+BC_SRC\0")))

#define UC_GET_ROLE_STRING(role)\
        ((GA_ROLE_SOURCE == role)?("SRC\0"):((GA_ROLE_SINK == role)?("SNK\0"):("Unknown\0")))

/* Invalid Source_ID. Used for rx_state */
#define APPL_BASS_INVALID_SRC_ID                 0xFFU

#endif /* BAP_BROADCAST */

#if (defined AUDIO_SRC_ISO_SUPPORT) || (defined AUDIO_SNK_ISO_SUPPORT)

#define AUDIO_ISO_IDLE                                 0x01U
#define AUDIO_ISO_UPDATE_IN_PROGRESS                   0x02U
#define AUDIO_ISO_SETUP_COMPLETE                       0x03U

#define AUDIO_SRC_ISO_MAX_ENTRIES                      5U
#define AUDIO_SNK_ISO_MAX_ENTRIES                      2U

/* Operations in ISO - Setup/Remove */
#define AUDIO_ISO_DATA_SETUP                           0x01U
#define AUDIO_ISO_DATA_REMOVE                          0x02U

/*
 * Data_Path_Direction for ISO Controller Commands,
 * Refer Core_v5.2, LE Setup ISO Data Path command.
 */
 /* Host to Controller - Refers to Source Only */
#define AUDIO_ISO_INPUT_H_TO_C                 0x00U
/* Host to Controller - Refers to Sink Only */
#define AUDIO_ISO_OUTPUT_C_TO_H                0x01U

/*
 * Data_Path_Direction for ISO Controller Commands,
 * Refer Core_v5.2, LE Remove ISO Data Path command.
 * Bit 0: Remove input data path
 * Bit 1: Remove output data path
 */
 /* Host to Controller - Refers to Source Only */
#define AUDIO_ISO_INPUT_DATA_PATH              (0x01U << 0U)
/* Host to Controller - Refers to Sink Only */
#define AUDIO_ISO_OUTPUT_DATA_PATH             (0x01U << 1U)

#define AUDIO_CODING_FORMAT_TRANSPARENT        0x03U

/* Data path shall be over the HCI transport */
#define AUDIO_DATA_PATH_ID_HCI                 0x00U

/*
 * When Data_Path_Direction is set to 0x00 (input), the Controller_Delay
 * parameter specifies the delay at the data source from the reference time of
 * an SDU to the CIG reference point (see [Vol 6] Part B, Section 4.5.14.1) or
 *  BIG anchor point (see [Vol 6] Part B, Section 4.4.6.4).
 * When Data_Path_Direction is set to 0x01 (output), Controller_Delay specifies
 * the delay from the CIG synchronization point or BIG synchronization point to
 * the point in time at which the Controller begins to transfer the
 * corresponding data to the data path interface. The Host should use the
 * HCI_Read_Local_Supported_Controller_Delay command to obtain a suitable
 * value for Controller_Delay.
 * Range: 0x000000 to 0x3D0900
 */
#define AUDIO_ISO_CONTROLLER_DELAY             0U

#define AUDIO_CIS_CONN_HANDLE_INVALID          0xFFFFU

/* Use for Ignore Value */
#define AUDIO_IGNORE_VALUE                     0xFFU

typedef struct _APPL_GA_UTILS_AUDIO_LOC_POS
{
    UINT32  location;
    UINT8   position;
}APPL_GA_UTILS_AUDIO_LOC_POS;

typedef struct _APPL_GA_UTILS_AUDIO_ACA_INFO
{
    UINT8  cc;
    APPL_GA_UTILS_AUDIO_LOC_POS aca_lp[AUDIO_SRC_LC3_CHANNELS_MAX];
}APPL_GA_UTILS_AUDIO_ACA_INFO;

typedef struct _AUDIO_ISO_INFO
{
    /* CIS/BIS Connection Handle */
    UINT16  conn_handle;
    /* CS Conf */
    GA_LC3_CODEC_SPECIFIC_CONF cs_conf;
    /* If CC > 1, ACA as per Priority List */
    APPL_GA_UTILS_AUDIO_ACA_INFO aca_info;
    /* State */
    UINT8   state;
    /* Streaming State */
    UINT8   streaming_state;
}AUDIO_ISO_INFO;
#endif /* (defined AUDIO_SRC_ISO_SUPPORT) || (defined AUDIO_SNK_ISO_SUPPORT) */

#if (defined AUDIO_SRC_PL_SUPPORT || defined AUDIO_SNK_PL_SUPPORT)

#define AUDIO_PL_IDLE                                 0x01U
#define AUDIO_PL_SETUP_COMPLETE                       0x02U
#define AUDIO_PL_STREAM_GENERATING                    0x03U
#define AUDIO_PL_STREAM_PLAYBACK                      0x04U

/*
 * NOTE: Currently Audio PL can support only one set of Audio Configuration at
 * a time for both Audio Generation and Playback at a point in time.
 * Hence, we shall support only one kind of LC3 configuration setting at a time
 * i.e.
 *
 * Possible Combinations:
 * A) Not Supported
 * ASE1 - CC 1 - FL
 * ASE2 - CC 1 - FR
 * B) Supported
 * ASE1 - CC 2 - FL + FR
 * ASE2 - CC 1 - FL
 * ASE3 - CC 1 - FR
 * C) Not Supported
 * ASE1 - CC 1 - FL
 * ASE2 - CC 2 - FL + FR
 *
 * During moving stream from Enabling to Streaming, Validate if the CC of the
 * ASE2 is less than or equal to ASE1 and the Audio Location is the same or a
 * subset of ASE1.
 * Otherwise, do not allow.
 * This inturn shall configure LC3 module to a single set of configuration.
 * Post LC3 Encoding, Validate all the Entries which are in streaming,
 * Based on the Channel Count and Audio Location, Stream the corresponding
 * encoded packets.
 */
typedef struct _AUDIO_PL_INFO
{
    /* Sampling Frequency in Hz the ASE is configured for */
    UINT16 sf_hz;

    /* Each Sample consists of bps number of bits */
    UCHAR bps;

    /* Channel Count the ASE is configured for */
    UCHAR cc;

    /* This paramater is unused for now in Audio PL, Give any value */
    UINT16 size;

    /* State of Audio PL */
    UINT8 state;

    /* Src Only: No of Bytes required for LC3 module to achieve the Bitrate */
    UINT16 bytes_to_lc3_encode;
}AUDIO_PL_INFO;

#endif /* (defined AUDIO_SRC_PL_SUPPORT || defined AUDIO_SNK_PL_SUPPORT) */

#if (defined AUDIO_SRC_LC3_SUPPORT || defined AUDIO_SNK_LC3_SUPPORT)

#define AUDIO_LC3_IDLE                          0x01U
#define AUDIO_LC3_CREATED                       0x02U
#define AUDIO_LC3_IN_PROGRESS                   0x03U

/** LC3 maximum number of supported samples */
#define AUDIO_LC3_INPUT_FRAME_SIZE_MAX         LC3_INPUT_FRAME_SIZE_MAX
/** LC3 maximum supported frame-size */
#define AUDIO_LC3_FRAME_SIZE_MAX               LC3_FRAME_SIZE_MAX

#ifdef AUDIO_SRC_LC3_SUPPORT
/** LC3 Encoder Maximum Core Buffer Size */
#define AUDIO_LC3_ENCODER_CORE_BUFFER_SIZE_MAX LC3_ENCODER_CORE_BUFFER_SIZE_MAX
/** LC3 Encoder Maximum Work Buffer Size */
#define AUDIO_LC3_ENCODER_WORK_BUFFER_SIZE_MAX LC3_ENCODER_WORK_BUFFER_SIZE_MAX
#endif /* AUDIO_SRC_LC3_SUPPORT */

#ifdef AUDIO_SNK_LC3_SUPPORT

/* Type of PLC method, 0=default */
#define AUDIO_LC3_DEC_PLC_TYPE_DEFAULT 0U

/* Playback Silence Buffer initially when the Stream has started streaming */
#define AUDIO_SNK_LC3_PLAYBACK_SILENCE_BUFFER_COUNT 4U

/** LC3 Decoder Maximum Core Buffer Size */
#define AUDIO_LC3_DECODER_CORE_BUFFER_SIZE_MAX LC3_DECODER_CORE_BUFFER_SIZE_MAX   /* 5216 */
/** LC3 Decoder Maximum Work Buffer Size */
#define AUDIO_LC3_DECODER_WORK_BUFFER_SIZE_MAX LC3_DECODER_WORK_BUFFER_SIZE_MAX   /* 6512 */
#endif /* AUDIO_SNK_LC3_SUPPORT */

/*
 * Ethermind LC3 module requires Frame_Duration in ms in the format below:
 * Frame duration in ms [100 for 10.0ms, 75 for 7.5ms]
 * Pass fd in MicroSeconds.
 * Eg: 10000us = 10ms, 100 as per LC3
 *     7500us  = 7.5ms, 75 as per LC3
 *
 * LC3 expects value 75 for 7.5ms
 * So, (7500us / 1000) = 7.5ms (Float Variable)
 * 7.5 * 10 = 75
 * As we can't have float variable,
 * (7500us / 100) = 75
 */
#define GET_SUPP_FRAME_DURATION_FOR_LC3(fd_in_us, fd_in_lc3_format)\
        ((fd_in_lc3_format) = (fd_in_us)/(100U))

#ifdef AUDIO_SNK_LC3_SUPPORT
typedef struct _AUDIO_LC3_SNK_CTX
{
    /* Parameters related to EtherMind LC3 Decoder */
    LC3_DECODER_CNTX  dec_ctx;

    /* LC3 Encoded Buffer to be used as input */
    UINT8  snk_enc_buffer[AUDIO_SNK_LC3_CHANNELS_MAX][AUDIO_LC3_FRAME_SIZE_MAX];

    /* LC3 Decoded Buffer PCM buffer to be filled with decoded audio samples */
    INT32  snk_dec_buffer[AUDIO_SNK_LC3_CHANNELS_MAX][AUDIO_LC3_INPUT_FRAME_SIZE_MAX];

    /* Core Buffer to be fed to LC3 Decoder Module */
    UINT8  dec_core_buffer[AUDIO_SNK_LC3_CHANNELS_MAX * AUDIO_LC3_DECODER_CORE_BUFFER_SIZE_MAX];
    /* Work Buffer to be fed to LC3 Decoder Module */
    UINT8  dec_work_buffer[AUDIO_SNK_LC3_CHANNELS_MAX * AUDIO_LC3_DECODER_WORK_BUFFER_SIZE_MAX];

    /*
     * LC3 Encoded Buffer Pointers holding enc_buffer references
     * Encoded buffer to be filled by the encoder.
     */
    UINT8* snk_enc_buf[AUDIO_SNK_LC3_CHANNELS_MAX];

    /* LC3 Decoded Buffer Pointers holding dec_buffer references */
    INT32* snk_dec_buf[AUDIO_SNK_LC3_CHANNELS_MAX];

} AUDIO_LC3_SNK_CTX;
#endif /* AUDIO_SNK_LC3_SUPPORT */

#ifdef AUDIO_SRC_LC3_SUPPORT
typedef struct _AUDIO_LC3_SRC_CTX
{
    /* Parameters related to EtherMind LC3 Encoder */
    /* LC3 Encoder Context */
    LC3_ENCODER_CNTX enc_ctx;

    /* Source PCM Buffer to be fed to LC3 Encoder Module */
    INT32  src_pcm_buffer[AUDIO_SRC_LC3_CHANNELS_MAX][AUDIO_LC3_INPUT_FRAME_SIZE_MAX];
    /* LC3 Encoded Buffer */
    UINT8  src_enc_buffer[AUDIO_SRC_LC3_CHANNELS_MAX][AUDIO_LC3_FRAME_SIZE_MAX];

    /* Target bytes [w.r.t bitrate] for each channel */
    INT32 target_enc_bytes[AUDIO_SRC_LC3_CHANNELS_MAX];

    /* Core Buffer to be fed to LC3 Encoder Module */
    UINT8  enc_core_buffer[AUDIO_SRC_LC3_CHANNELS_MAX * AUDIO_LC3_ENCODER_CORE_BUFFER_SIZE_MAX];
    /* Work Buffer to be fed to LC3 Encoder Module */
    UINT8  enc_work_buffer[AUDIO_SRC_LC3_CHANNELS_MAX * AUDIO_LC3_ENCODER_WORK_BUFFER_SIZE_MAX];

    /*
     * Source Buffer Pointers holding pcm_buffer references
     * PCM buffer containing input audio samples.
     */
    INT32 * src_pcm_buf[AUDIO_SRC_LC3_CHANNELS_MAX];
    /*
     * LC3 Encoded Buffer Pointers holding enc_buffer references
     * Encoded buffer to be filled by the encoder.
     */
    UINT8 * src_enc_buf[AUDIO_SRC_LC3_CHANNELS_MAX];

} AUDIO_LC3_SRC_CTX;
#endif /* AUDIO_SRC_LC3_SUPPORT */

typedef struct _AUDIO_LC3_SNK_INFO
{
    /* Sampling Frequency in Hz */
    UINT16 sf_hz;

    /* Each Sample consists of bps number of bits */
    UCHAR bps;

    /* Channel Count */
    UINT8  cc;

    /* Frame Duration in MicroSeconds */
    UINT16  fd_in_us;

    /* Audio Locations - OR Values of > 1 if CC > 1 */
    UINT32 location;

    /* State of Audio LC3 */
    UINT8 state;

    /* If CC > 1, ACA as per Priority position List */
    APPL_GA_UTILS_AUDIO_ACA_INFO aca_info;

    /* Sink Context */
    AUDIO_LC3_SNK_CTX ctx;
}AUDIO_LC3_SNK_INFO;

typedef struct _AUDIO_LC3_SRC_INFO
{
    /* Sampling Frequency in Hz */
    UINT16 sf_hz;

    /* Each Sample consists of bps number of bits */
    UCHAR bps;

    /* Channel Count */
    UINT8  cc;

    /* Frame Duration in MicroSeconds */
    UINT16  fd_in_us;

    /* Audio Locations - OR Values of > 1 if CC > 1 */
    UINT32 location;

    /* State of Audio LC3 */
    UINT8 state;

    /* If CC > 1, ACA as per Priority position List */
    APPL_GA_UTILS_AUDIO_ACA_INFO aca_info;

    /* Source Context */
    AUDIO_LC3_SRC_CTX ctx;
}AUDIO_LC3_SRC_INFO;

#endif /* (defined AUDIO_SRC_LC3_SUPPORT || defined AUDIO_SNK_LC3_SUPPORT) */

/* Initializer API for Device manager */
void appl_ga_utils_dev_manager_init(void);

/* Initializer API for PACS data */
void appl_ga_utils_pacs_init(void);

/* Parser for printing GA_RESULT */
GA_RESULT appl_ga_utils_display_ga_result(GA_RESULT result);

/* Parser for printing Service UUID */
void appl_ga_utils_display_svc_uuid(UINT16 svc_uuid);

/* Parser for printing Service Info - Start Range & End Range */
void appl_ga_utils_display_svc_range
     (
         GA_BRR_SVC_RANGE range
     );

/* Parser for printing Characteristic UUID */
void appl_ga_utils_display_char_uuid(UINT16 char_uuid);

/* Parser for printing Mute Value - common for MICP and VCP */
void appl_ga_utils_display_mute(UINT8 mute);

/* Parser for printing Audio Location */
void appl_ga_utils_display_audio_loc(UINT32 audio_loc);

void appl_ga_utils_bap_display_role(UINT8 role);

void appl_ga_utils_bap_display_cap_type(UINT8 type);

void appl_ga_utils_bap_parse_and_fill_codec_id
     (
         /* IN */   UCHAR * array,
         /* IN */   UINT8   array_len,
         /* OUT */  GA_CODEC_INFO * codec_id
     );

void appl_ga_utils_bap_display_codec_id(GA_CODEC_INFO codec_id);

void appl_ga_utils_bap_display_sf(UINT8 sf);

void appl_ga_utils_bap_display_fd(UINT8 fd);

void appl_ga_utils_bap_parse_and_display_codec_spec_cap_ltv
     (
         /* IN */   UCHAR * ltvarray,
         /* IN */   UINT8   ltvarray_len,
         /* OUT */  GA_LC3_CODEC_SPECIFIC_CAP * codec_specific_cap
     );

void appl_ga_utils_bap_parse_and_display_codec_spec_conf_ltv
     (
         /* IN */   UCHAR * ltvarray,
         /* IN */   UINT8   ltvarray_len,
         /* OUT */  GA_LC3_CODEC_SPECIFIC_CONF * codec_specific_conf
     );

void appl_ga_utils_bap_parse_and_display_metadata_ltv
     (
         /* IN */    UCHAR  * ltvarray,
         /* IN */    UINT8    ltvarray_len,
         /* INOUT */ UINT16 * pref_audio_ctx,
         /* INOUT */ UINT16 * streaming_audio_ctx
     );

void appl_ga_utils_bap_display_audio_locations(UINT32 audio_locations);

void appl_ga_utils_bap_display_ctx(UINT16 contexts);

void appl_ga_utils_bap_display_sacc(UINT8 sacc);

void appl_ga_utils_bap_display_sfd(UINT8 sfd);

void appl_ga_utils_bap_display_ssf(UINT16 ssf);

void appl_ga_utils_bap_display_coding_format(UINT8 coding_format);

void appl_ga_utils_bap_display_cs_cap_set_for_input(void);
UINT8 appl_ga_utils_bap_get_cs_cap_set_id(void);

void appl_ga_utils_bap_display_audio_channel_count_for_input(void);

void appl_ga_utils_bap_display_framing(UINT8 framing);

void appl_ga_utils_bap_display_target_latency(UINT8 target_latency);
void appl_ga_utils_bap_display_pref_phy(UINT8 pref_phy);
void appl_ga_utils_bap_display_target_phy(UINT8 target_phy);

void appl_ga_utils_bap_display_ctx_type_for_input(void);

void appl_ga_utils_bap_display_audio_locations_for_input(void);

void appl_ga_utils_bap_display_target_latency_for_input(void);
void appl_ga_utils_bap_display_target_phy_for_input(void);
void appl_ga_utils_bap_display_pref_phy_for_input(void);

void appl_ga_utils_bap_uc_display_qos_cs_cap_set_for_input(void);

void appl_ga_utils_bap_bc_display_qos_cs_cap_set_for_input(void);

GA_RESULT appl_ga_utils_bap_prepare_cs_cap_based_on_set_id
          (
              UINT8 csc_set_id,
              GA_LC3_CODEC_SPECIFIC_CAP * csc
          );

void appl_ga_utils_bap_create_lc3_codec_spec_cap_ltv
     (
         /* IN */   GA_LC3_CODEC_SPECIFIC_CAP * codec_specific_cap,
         /* OUT */  UCHAR * ltvarray,
         /* OUT */  UINT8 * ltvarray_len
     );

void appl_ga_utils_bap_get_vendor_spec_codec_spec_cap_ltv
     (
         /* OUT */  UCHAR * ltvarray,
         /* OUT */  UINT8 * ltvarray_len
     );

void appl_ga_utils_bap_get_vendor_spec_codec_spec_conf_ltv
     (
         /* OUT */  UCHAR * ltvarray,
         /* OUT */  UINT8 * ltvarray_len
     );

void appl_ga_utils_bap_create_lc3_codec_spec_conf_ltv
     (
         /* IN */  GA_LC3_CODEC_SPECIFIC_CONF * codec_specific_conf,
         /* OUT */ UCHAR                      * ltvarray,
         /* OUT */ UINT8                      * ltvarray_len
     );

GA_RESULT appl_ga_utils_bap_uc_prepare_csc_and_qos_cap_based_on_set_id
          (
              UINT8                        csc_set_id,
              GA_LC3_CODEC_SPECIFIC_CONF * csc,
              GA_QOS_CAP                 * qos_cap,
              GA_QOS_CONF                * qos_conf
          );

GA_RESULT appl_ga_utils_bap_bc_prepare_csc_and_qos_conf_based_on_set_id
          (
              UINT8                        csc_set_id,
              GA_LC3_CODEC_SPECIFIC_CONF * csc,
              GA_QOS_CONF                * qos_cap
          );

void appl_ga_utils_bap_display_language_codes_for_input(void);

int appl_ga_utils_bap_get_language_code(void);

void appl_ga_utils_set_language_code(UINT32* lang_code, UINT8 choice);

void appl_ga_utils_display_bdaddr_and_type(GA_ENDPOINT * device);

void appl_ga_utils_bap_parse_and_display_qos_cnf(GA_QOS_CONF qos_conf);

UINT8 appl_ga_utils_bap_get_target_phy(void);
UINT8 appl_ga_utils_bap_get_pref_phy(void);

UINT8 appl_ga_utils_bap_get_pref_presentation_delay(void);

UINT8 appl_ga_utils_bap_compare_codec_spec_conf
      (
          GA_LC3_CODEC_SPECIFIC_CONF * codec_spec_conf_1,
          GA_LC3_CODEC_SPECIFIC_CONF * codec_spec_conf_2
      );

UINT32 appl_ga_utils_bap_fetch_sf(UINT8 sf);

UINT8 appl_ga_utils_fetch_sacc_from_aca(UINT32 aca);

void appl_ga_utils_fetch_priority_from_aca
     (
         UINT32 aca,
         APPL_GA_UTILS_AUDIO_ACA_INFO * aca_info
     );

UINT32 appl_ga_utils_bap_fetch_bitrate_from_qos_conf_set_id
       (
           UINT8 qos_conf_set_id
       );

UINT32 appl_ga_utils_bap_get_audio_channel_alloc(void);

UINT16 appl_ga_utils_bap_fetch_streaming_audio_ctx(void);
UINT16 appl_ga_utils_bap_get_streaming_audio_ctx(void);
UINT16 appl_ga_utils_bap_get_supp_ctx(void);
UINT16 appl_ga_utils_bap_get_available_ctx(void);
UINT16 appl_ga_utils_bap_get_ctx_type(void);

UINT8 appl_ga_utils_bap_get_parental_rating_type(void);

void appl_ga_utils_bap_get_input_and_create_metadata_ltv
     (
         /* OUT */ UCHAR                      * ltvarray,
         /* OUT */ UINT8                      * ltvarray_len
     );

void appl_ga_utils_bap_display_packing(UINT8 packing);

UINT8 appl_ga_utils_bap_get_codec_frame_block_per_sdu(void);

UINT8 appl_ga_utils_bap_bc_get_qos_cs_cap_set_id(void);

void appl_ga_utils_bap_set_codec_id_for_lc3_defined_csc_set_id
     (
         /* OUT */ GA_CODEC_INFO * codec_info
     );

void appl_ga_utils_bap_get_codec_id_for_vendor_spec
     (
         /* OUT */ GA_CODEC_INFO * codec_info
     );

UINT8 appl_ga_utils_bap_get_audio_channel_count(void);

UINT8 appl_ga_utils_bap_get_max_codec_frames_per_sdu(void);

UINT8 appl_ga_utils_bap_get_choice_for_update_notify(void);

UINT8 appl_ga_utils_bap_get_role(void);

UINT8 appl_ga_utils_bap_get_pacs_cap_count(void);

UINT8 appl_ga_utils_bap_get_pacs_cap_set_count(void);

UINT8 appl_ga_utils_bap_get_ase_count(void);

UINT8 appl_ga_utils_bap_get_ccid_count(void);

UINT8 appl_ga_utils_bap_get_ccid(void);

void appl_ga_utils_display_ccid(void);

GA_RESULT appl_ga_register_pacs(APPL_GA_ROLE role);

#ifndef APPL_GA_USE_CONFIG_FILE_FOR_INPUT
GA_RESULT appl_ga_utils_bap_register_pacs_using_runtime_values(void);
#else /* APPL_GA_USE_CONFIG_FILE_FOR_INPUT */
GA_RESULT appl_ga_utils_bap_register_pacs_using_default_values
          (
              /* IN */  APPL_GA_ROLE  role,
              /* IN  */ UINT8         cap_role
          );
#endif /* APPL_GA_USE_CONFIG_FILE_FOR_INPUT */
UINT8 appl_ga_utils_bap_get_cap_record_id(void);
void appl_ga_utils_bap_get_cap(UINT8 role, GA_AUDIO_CAP* cap, UINT8* record_id);
void appl_ga_utils_bap_remove_pacs_cap(void);

GA_RESULT appl_ga_utils_add_conn_device_as_a_initiator(GA_ENDPOINT device);

GA_RESULT appl_ga_utils_get_dev_from_handle
          (
              UINT16        conn_handle,
              GA_ENDPOINT * device
          );

GA_RESULT appl_ga_utils_get_dev_handle_from_device
          (
            GA_ENDPOINT   device,
            UINT16      * conn_handle
          );

GA_RESULT appl_ga_utils_bap_add_remote_pacs_for_dev
          (
              GA_ENDPOINT     * device,
              GA_CAP_ENDPOINT * capab
          );

GA_RESULT appl_ga_utils_bap_remove_remote_pacs_for_dev(UINT8 conn_dev_index);
GA_RESULT appl_ga_utils_bap_fetch_remote_pacs_based_on_dev
          (
              GA_ENDPOINT     device,
              PACS_CAP_INFO * pacs
          );

GA_RESULT appl_ga_utils_bap_fetch_remote_free_entry_for_pacs(UINT8 * index);

void appl_ga_utils_bap_fetch_local_pacs(PACS_CAP_INFO * pacs);

void appl_ga_utils_bap_display_local_pacs(void);

void appl_ga_utils_bap_display_remote_pacs(UINT8 conn_dev_index);

void appl_ga_utils_bap_display_avail_and_supp_pacs_ctx
     (
         UINT8                role,
         GA_PACS_AUDIO_RECORD pacs_audio_record
     );

GA_RESULT appl_ga_utils_bap_get_remote_pacs_index
          (
              UINT8   conn_dev_index,
              UINT8 * index
          );

void appl_ga_utils_bap_fetch_update_local_pacs_audio_locations
     (
         UINT8 role,
         UINT32 locations
     );

void appl_ga_utils_bap_parse_and_display_audio_cap_codecs
     (
         UINT8    index,
         UINT8    role,
         UINT8  * cap,
         UINT16   cap_len
     );

void appl_ga_utils_bap_parse_and_display_audio_cap_locations
     (
         UINT8    index,
         UINT8    role,
         UINT8  * cap,
         UINT16   cap_len
     );

void appl_ga_utils_bap_parse_and_display_audio_cap_avail_ctx
     (
         UINT8    index,
         UINT8    role,
         UINT8  * cap,
         UINT16   cap_len
     );

void appl_ga_utils_bap_parse_and_display_audio_cap_supp_ctx
     (
         UINT8    index,
         UINT8    role,
         UINT8  * cap,
         UINT16   cap_len
     );

GA_RESULT appl_ga_utils_get_dev_index_from_handle
          (
              UINT16  conn_handle,
              UINT8 * index
          );
GA_RESULT appl_ga_utils_handle_conn_device(GA_ENDPOINT device, GA_RESULT status, UINT16 handle);
GA_RESULT appl_ga_utils_remove_conn_device(UINT8 index);
void appl_ga_utils_display_all_conn_device(void);
GA_RESULT appl_ga_utils_set_conn_device_in_use(UINT8 index);
GA_RESULT appl_ga_utils_get_free_conn_device(UINT8 * index);
void appl_ga_utils_get_conn_device_in_use
     (
         UINT8       * index,
         GA_ENDPOINT * device,
         GA_CONTEXT  * context
     );
GA_RESULT appl_ga_utils_get_conn_handle_from_index
          (
              UINT8    conn_dev_index,
              UINT16 * acl_handle
          );
void appl_ga_utils_update_context_of_conn_dev
     (
         GA_CONTEXT context,
         UINT8 index
     );
GA_RESULT appl_ga_utils_get_conn_device(UINT8 index, GA_ENDPOINT * device);
GA_RESULT appl_ga_utils_get_conn_device_index(GA_ENDPOINT device, UINT8 * index);

void appl_ga_utils_bap_display_cs_conf_set_for_input(void);
UINT8 appl_ga_utils_bap_get_cs_conf_id(void);
UINT8 appl_ga_utils_bap_uc_get_qos_cs_cap_set_id(void);

void appl_ga_utils_connect(GA_ENDPOINT device, UCHAR mode);

GA_RESULT appl_ga_utils_disconnect
          (
              UINT16  connection_handle,
              UCHAR   reason
          );

void appl_ga_utils_bap_display_ase_id_for_input(void);
UINT8 appl_ga_utils_bap_get_ase_id(void);
void appl_ga_utils_bap_display_cis_id_for_input(void);
UINT8 appl_ga_utils_bap_get_cis_id(void);
void appl_ga_utils_bap_display_cig_id_for_input(void);
UINT8 appl_ga_utils_bap_get_cig_id(void);
UINT8 appl_ga_utils_bap_get_data_flow_direction(void);
void appl_ga_utils_bap_display_data_flow_direction_for_input(void);
UINT8 appl_ga_utils_bap_get_input_cis_count(void);
UINT8 appl_ga_utils_bap_get_input_conn_dev_index(void);

void appl_ga_utils_bap_parse_and_display_ase_state(UCHAR ase_state);

void appl_ga_utils_bap_display_adv_set_identifier(void);
UINT8 appl_ga_utils_bap_get_adv_set_identifier(void);
void appl_ga_utils_bap_display_packing_for_input(void);
UINT8 appl_ga_utils_bap_get_packing(void);
void appl_ga_utils_bap_display_sca_for_input(void);
UINT8 appl_ga_utils_bap_get_sca(void);
void appl_ga_utils_bap_get_program_info(UCHAR * data, UINT8 * datalen);
void appl_ga_utils_bap_display_program_info(void);
void appl_ga_utils_bap_get_program_info_url(UCHAR * data, UINT8 * datalen);
void appl_ga_utils_bap_display_program_info_url(void);
void appl_ga_utils_bap_get_extended_metadata(UCHAR * data, UINT8 * datalen);
void appl_ga_utils_bap_display_extended_metadata(void);
void appl_ga_utils_bap_get_vendor_specific(UCHAR * data, UINT8 * datalen);
void appl_ga_utils_bap_display_vendor_specific(void);
void appl_ga_utils_bap_display_audio_src_appearance_value_for_input(void);
UINT16 appl_ga_utils_bap_get_audio_src_appearance_value(void);
#ifdef GA_PBP
void appl_ga_utils_bap_display_pbp_audio_conf_type_for_input(void);
UINT8 appl_ga_utils_bap_get_pbp_audio_conf_type(void);
#endif /* GA_PBP */

UINT16 appl_ga_utils_get_fd_in_us(UINT8 fd_in_ms);

void appl_ga_utils_pacs_display_char_uuid(UINT16 char_uuid);
void appl_ga_utils_ascs_display_char_uuid(UINT16 char_uuid);
void appl_ga_utils_bap_display_ase_data(UINT8 conn_dev_index, ASE ase);
void appl_ga_utils_bap_display_local_ase_db(ASE ase);
void appl_ga_utils_bap_parse_and_display_iso_state(UINT8 iso_state);
void appl_ga_utils_bap_parse_and_display_cig_state(UINT8 cig_state);
void appl_ga_utils_bap_parse_and_display_cis_state(UINT8 cis_state);
UINT8 appl_ga_utils_fetch_qos_conf_set_id_from_qos_conf(GA_QOS_CONF qos_conf);

GA_RESULT appl_ga_utils_enable_legacy_advertise(UINT8 enable);
GA_RESULT appl_ga_utils_enable_extended_advertise(UINT8 enable);
void appl_ga_utils_set_adv_data_for_current_tmap_role(APPL_GA_ROLE tmap_role);
void appl_ga_utils_set_adv_data_for_current_hap_role
     (
         APPL_GA_ROLE  hap_role,
         UINT8         hap_features
     );
GA_RESULT appl_ga_utils_enable_legacy_scan(UINT8 enable);

void appl_ga_utils_ucs_ascs_display_common_event_info
     (
        GA_ENDPOINT * device,
        UINT16        ga_status
     );

void appl_ga_utils_ucc_ascs_display_common_event_info
     (
        GA_ENDPOINT * device,
        UINT8         ga_event,
        UINT16        ga_status
     );
void appl_ga_utils_ucc_ascs_display_event_type(UINT8 ga_event);

void appl_ga_utils_ucc_ascs_parse_and_display_ase_data(UCHAR * data, UINT16 datalen);
void appl_ga_utils_ucc_ascs_parse_and_display_ase_params_codec_cnf
     (
         UCHAR * data,
         UINT16  datalen,
         UINT8   capability
     );
void appl_ga_utils_ucc_ascs_parse_and_display_ase_params_qos_cnf
     (
         UCHAR * data,
         UINT16  datalen
     );
void appl_ga_utils_ucc_ascs_parse_and_display_ase_params_enabling_streaming_disabling
     (
         UCHAR * data,
         UINT16  datalen
     );
void appl_ga_utils_ucc_ascs_parse_and_display_isoal_framing(UINT8 framing);

void appl_ga_utils_ucc_ascs_display_cp_event_data
     (
         UINT16        event_status,
         GA_ASE_INFO * ase
     );
void appl_ga_utils_ucc_ascs_parse_and_display_asecp_rsp_rsn_code
     (
        UINT8 asecp_rsp_code,
        UINT8 asecp_rsn_code
     );
void appl_ga_utils_ucc_ascs_parse_and_display_asecp_reason_code_for_config_reason
     (
         UINT8 asecp_rsp_reasn_code
     );
void appl_ga_utils_ucc_ascs_parse_and_display_asecp_reason_code_for_metadata_reason
     (
         UINT8 asecp_rsp_reasn_code
     );
void appl_ga_utils_bc_display_src_console_header
     (
         GA_ENDPOINT * device,
         UINT16        ga_status
     );
void appl_ga_utils_bc_display_sink_console_header
     (
         GA_ENDPOINT * device,
         UINT16        ga_status
     );

void appl_ga_utils_parse_and_display_ea_report_subevent
     (
         UCHAR * ga_event_data,
         UINT8   ga_event_datalen
     );
void appl_ga_utils_parse_and_display_ea_report_subevent_data
     (
         UCHAR * ga_event_data,
         UINT8   ga_event_datalen
     );

void appl_ga_utils_parse_and_display_adv_data
     (
         UCHAR * adv_data,
         UINT8   adv_datalen
     );

GA_RESULT appl_ga_utils_enable_extended_scan(GA_BRR_EXTENDED_SCAN_PARAMS escan_params);
GA_RESULT appl_ga_utils_disable_extended_scan(GA_BRR_EXTENDED_SCAN_PARAMS escan_params);

#endif /* _H_APPL_GA_UTILS_ */
