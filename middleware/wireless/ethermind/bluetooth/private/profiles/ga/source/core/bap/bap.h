
/**
 *  \file bap.h
 *
 *  \brief This file defines the Basic Audio Profile Interface - includes
 *  Data Structures and Methods.
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BAP_
#define _H_BAP_


/* --------------------------------------------- Header File Inclusion */
#include "GA_common.h"
#include "GA_bearer_api.h"
#include "GA_property_config.h"

/* --------------------------------------------- Global Definitions */
/** BAP Service Indices */
#define BAP_SERVICE_PACS                        0U
#define BAP_SERVICE_ASCS                        1U

#define BAP_SERVICE_COUNT                       2U

/** BAP Unicast and Broadcast Roles */
#define BAP_ROLE_CLIENT                         0x01U
#define BAP_ROLE_SERVER                         0x02U
#define BAP_ROLE_BCSRC                          0x04U
#define BAP_ROLE_BCSNK                          0x08U

/** BAP Audio Endpoint Roles */
#define BAP_ROLE_SINK                           0x01U
#define BAP_ROLE_SOURCE                         0x02U

/** SEP States */
#define SEP_STATE_IDLE                          0x00U
#define SEP_STATE_CODEC_CONFIGURED              0x01U
#define SEP_STATE_QOS_CONFIGURED                0x02U
#define SEP_STATE_ENABLING                      0x03U
#define SEP_STATE_STREAMING                     0x04U
#define SEP_STATE_DISABLING                     0x05U
#define SEP_STATE_RELEASING                     0x06U

#define SEP_STATE_INVALID                       0xFFU

/** ASE Control Point Opcodes */
#define BAP_ASECP_CONFIG_CODEC                  0x01U
#define BAP_ASECP_CONFIG_QOS                    0x02U
#define BAP_ASECP_ENABLE                        0x03U
#define BAP_ASECP_RECEIVER_START_READY          0x04U
#define BAP_ASECP_DISABLE                       0x05U
#define BAP_ASECP_RECEIVER_STOP_READY           0x06U
#define BAP_ASECP_UPDATE_METADATA               0x07U
#define BAP_ASECP_RELEASE                       0x08U

/** Events from BAP */
#define BAP_SETUP_CNF                           0x01U
#define BAP_RELEASE_CNF                         0x02U
#define BAP_CAPABILITY_STATUS_CNF               0x03U
#define BAP_LOCATION_STATUS_CNF                 0x04U
#define BAP_SUPPORTED_CONTEXTS_STATUS_CNF       0x05U
#define BAP_AVAILABLE_CONTEXTS_STATUS_CNF       0x06U
#define BAP_ASE_DISCOVER_CNF                    0x07U
#define BAP_ASE_STATUS_CNF                      0x08U
#define BAP_ASECP_RSP                           0x09U

#define BAP_ASE_SET_IND                         0x11U
#define BAP_ASE_GET_IND                         0x12U
#define BAP_ASE_STATUS_IND                      0x13U
#define BAP_ASE_RX_START_READY_IND              0x14U
#define BAP_ASE_RX_STOP_READY_IND               0x15U

#define BAP_SRC_CAPABILITY_STATUS_NTF           0x16U
#define BAP_SNK_CAPABILITY_STATUS_NTF           0x17U
#define BAP_SRC_LOCATION_STATUS_NTF             0x18U
#define BAP_SNK_LOCATION_STATUS_NTF             0x19U
#define BAP_SUPPORTED_CONTEXTS_STATUS_NTF       0x20U
#define BAP_AVAILABLE_CONTEXTS_STATUS_NTF       0x21U

#define BAP_PACS_SNK_LOCATIONS_SET_IND          0x22U
#define BAP_PACS_SRC_LOCATIONS_SET_IND          0x23U

/* BAP Broadcast Event Code */
#define BAP_BROADCAST_EVENT_CODE                GA_BRR_CONNECTIONLESS_EVENT_CODE

/** BAP Broadcast events */
#define BAP_BC_SETUP_ANNOUNCEMENT_CNF           (0x00U | BAP_BROADCAST_EVENT_CODE)
#define BAP_BC_END_ANNOUNCEMENT_CNF             (0x01U | BAP_BROADCAST_EVENT_CODE)
#define BAP_BC_START_CNF                        (0x02U | BAP_BROADCAST_EVENT_CODE)
#define BAP_BC_SUSPEND_CNF                      (0x03U | BAP_BROADCAST_EVENT_CODE)
#define BAP_BC_SUSPEND_IND                      (0x04U | BAP_BROADCAST_EVENT_CODE)

#define BAP_BC_SCAN_ANNOUNCEMENT_CNF            (0x05U | BAP_BROADCAST_EVENT_CODE)
#define BAP_BC_SCAN_END_CNF                     (0x06U | BAP_BROADCAST_EVENT_CODE)
#define BAP_BC_ASSOCIATE_CNF                    (0x07U | BAP_BROADCAST_EVENT_CODE)
#define BAP_BC_DISSOCIATE_CNF                   (0x08U | BAP_BROADCAST_EVENT_CODE)
#define BAP_BC_DISSOCIATE_IND                   (0x09U | BAP_BROADCAST_EVENT_CODE)
#define BAP_BC_ENABLE_CNF                       (0x0AU | BAP_BROADCAST_EVENT_CODE)
#define BAP_BC_DISABLE_CNF                      (0x0BU | BAP_BROADCAST_EVENT_CODE)
#define BAP_BC_DISABLE_IND                      (0x0CU | BAP_BROADCAST_EVENT_CODE)

#define BAP_BC_SOURCE_ANNOUNCEMENT_IND          (0x0DU | BAP_BROADCAST_EVENT_CODE)
#define BAP_BC_SOURCE_CONFIG_IND                (0x0EU | BAP_BROADCAST_EVENT_CODE)
#define BAP_BC_SOURCE_STREAM_IND                (0x0FU | BAP_BROADCAST_EVENT_CODE)

#define BAP_INVALID                             0xFFU
#define BAP_HANDLE_INVALID                      0xFFU

/** BAP ASE States */
#define BAP_ASE_STATE_INITIALIZED               0x00U
#define BAP_ASE_STATE_IDLE                      0x01U

/* Invalid Advertising Set Handle */
#define BAP_BC_INVALID_ADVSET_HANDLE            0xFFU

/* BAP Broadcast Code size */
#define BAP_BC_CODE_SIZE                        16U

/**
 *  \defgroup ga_bap_char_id BAP Char ID
 *  \{
 *  This section lists the Characteristic ID references.
 */

/** PACS Char ID: Sink PAC,
 * \ref GA_CHAR_PROP_PACS_SINK_PAC
 */
#define BAP_CHAR_ID_SINK_PAC                            0U
#define BAP_CHAR_ID_SINK_PAC_COUNT                      1U
/** PACS Char ID: Sink Audio Locations,
 * \ref GA_CHAR_PROP_PACS_SINK_AUDIO_LOCATIONS
 */
#define BAP_CHAR_ID_SINK_AUDIO_LOCATIONS                1U
#define BAP_CHAR_ID_SINK_AUDIO_LOCATIONS_COUNT          1U
/** PACS Char ID: Bearer Technology,
 * \ref GA_CHAR_PROP_TBS_BRR_TECHNOLOGY
 */
/** PACS Char ID: Source PAC,
 * \ref GA_CHAR_PROP_PACS_SOURCE_PAC
 */
#define BAP_CHAR_ID_SOURCE_PAC                          2U
#define BAP_CHAR_ID_SOURCE_PAC_COUNT                    1U
/** PACS Char ID: Source Audio Locations,
 * \ref GA_CHAR_PROP_PACS_SOURCE_AUDIO_LOCATIONS
 */
#define BAP_CHAR_ID_SOURCE_AUDIO_LOCATIONS              3U
#define BAP_CHAR_ID_SOURCE_AUDIO_LOCATIONS_COUNT        1U
/** PACS Char ID: Available Audio Contexts,
 * \ref GA_CHAR_PROP_PACS_AVAILABLE_AUDIO_CONTEXTS
 */
#define BAP_CHAR_ID_AVAILABLE_AUDIO_CONTEXTS            4U
#define BAP_CHAR_ID_AVAILABLE_AUDIO_CONTEXTS_COUNT      1U
/** PACS Char ID: Supported Audio Contexts,
 * \ref GA_CHAR_PROP_PACS_SUPPORTED_AUDIO_CONTEXTS
 */
#define BAP_CHAR_ID_SUPPORTED_AUDIO_CONTEXTS            5U
#define BAP_CHAR_ID_SUPPORTED_AUDIO_CONTEXTS_COUNT      1U

/** ASCS Char ID: ASE Control Point,
 * \ref GA_CHAR_PROP_ASCS_ASE_CP
 */
#define BAP_CHAR_ID_ASE_CP                              6U
#define BAP_CHAR_ID_ASE_CP_COUNT                        1U
/** ASCS Char ID: ASE Source,
 * \ref GA_CHAR_PROP_ASCS_SOURCE_ASE
 */
#define BAP_CHAR_ID_ASE_SOURCE                          7U
#define BAP_CHAR_ID_ASE_SOURCE_COUNT                    GA_CONFIG_LIMITS(GA_MAX_SOURCE_STREAM_CONTEXTS)
/** ASCS Char ID: ASE Sink,
 * \ref GA_CHAR_PROP_ASCS_SINK_ASE
 */
#define BAP_CHAR_ID_ASE_SINK                            8U
#define BAP_CHAR_ID_ASE_SINK_COUNT                      GA_CONFIG_LIMITS(GA_MAX_SINK_STREAM_CONTEXTS)

/** \} */

/**
 * \defgroup ga_bass_macros_broadcast_id Broadcast_ID
 * \{
 * Describes Broadcast_ID defined by the module.
 */
 /** Broadcast_ID Length */
#define BAP_BC_ID_LEN                           3U
/** \} */

/**
 * \defgroup ga_bap_char_id_count
 * \{
 * Describes count of char ID defined in PACS & ASCS Specification.
 */

/** Total number of Characteristic IDs,
 * \ref ga_pacs_char_prop
 * \ref ga_ascs_char_prop
 */
#define BAP_CHAR_ID_COUNT                               9U
/** \} */

/* Derive the characteristic config mask for the Char ID */
#define BAP_CHAR_CONFIG(id)                             (1U << (id))

/* --------------------------------------------- Data types /Structures */

/** BAP Handle for the core */
typedef UINT8 BAP_HANDLE;

/** PAC Record */
typedef struct _BAP_PAC_RECORD
{
    /* Codec Coding Format */
    UINT8 coding_format;

    /* Company ID */
    UINT16 company_id;

    /* Vendor Codec ID */
    UINT16 vendor_codec_id;

    /* Codec Capabilities */
    UCHAR codec_cap[GA_CODEC_IE_LEN];

    /* Codec Capabilities Length */
    UINT8 codec_caplen;

    /* Metadata information */
    UCHAR metadata[GA_METADATA_LEN];

    /* Metadata length */
    UINT8 metadata_len;

} BAP_PAC_RECORD;

/** BAP ASE Codec Configuration */
typedef struct _BAP_ASE_CODEC_CONF
{
    /* Role */
    UINT8 role;

    /* Target Latency */
    UINT8 target_latency;

    /* Target Phy */
    UINT8 target_phy;

    /* Codec Information */
    GA_CODEC_INFO codec;

} BAP_ASE_CODEC_CONF;

/** BAP Channel Reference */
typedef struct _BAP_CHANNEL
{
    /** CIG Identifier */
    UINT8 cig_id;

    /** CIS Identifier */
    UINT8 cis_id;

} BAP_CHANNEL;

/** BAP QOS Configuration */
typedef struct _BAP_ASE_QOS_CONF
{
    /** Channel Information */
    BAP_CHANNEL channel;

    /** SDU Interval */
    UINT32 sdu_interval;

    /** Framing */
    UINT8 framing;

    /** PHY */
    UINT8 phy;

    /** MAX SDU */
    UINT16 max_sdu;

    /** Retransmission Number */
    UINT8 retxn;

    /** Max Transport Latency */
    UINT16 max_transport_latency;

    /** Presentation Delay */
    UINT32 presentation_delay;

} BAP_ASE_QOS_CONF;

/** ASE Read operation structure */
typedef struct _BAP_ASE_READ
{
    /** ASE ID */
    UINT8 id;

    /** ASE State */
    UINT8 state;

    /** Data */
    UCHAR * data;

    /** Data Length */
    UINT8 length;

} BAP_ASE_READ;

/** Broadcast event structures for parameter decoding */
typedef struct _BAP_BC_START_PARAMS
{
    UINT8 error_code;
    UINT8 big_handle;
    UINT32 big_sync_delay;
    UINT32 transport_latency_big;
    UINT8 phy;
    UINT8 nse;
    UINT8 bn;
    UINT8 pto;
    UINT8 irc;
    UINT16 max_pdu;
    UINT16 iso_interval;
    UINT8 num_bis;
    UINT16 * connection_handle;
} BAP_BC_START_PARAMS;

typedef struct _BAP_BC_SUSPEND_PARAMS
{
    UINT8 big_handle;
    UINT8 reason;
} BAP_BC_SUSPEND_PARAMS;

typedef struct _BAP_BC_SOURCE_ANNOUNCEMENT_PARAMS
{
    UINT16 event_type;
    UINT8 address_type;
    UCHAR * address;
    UINT8 primary_phy;
    UINT8 secondary_phy;
    UINT8 advertising_sid;
    INT8 tx_power;
    INT8 rssi;
    UINT16 periodic_adv_interval;
    UINT8 direct_address_type;
    UCHAR * direct_address;
    UCHAR datalen;
    UCHAR * data;
} BAP_BC_SOURCE_ANNOUNCEMENT_PARAMS;

typedef struct _BAP_BC_SOURCE_CONFIG_PARAMS
{
    UINT16 sync_handle;
    INT8 tx_power;
    INT8 rssi;
    UINT8 cte_type;
    UINT8 data_status;
    UINT8 datalen;
    UCHAR * data;
} BAP_BC_SOURCE_CONFIG_PARAMS;

typedef struct _BAP_BC_SOURCE_STREAM_PARAMS
{
    UINT16 sync_handle;
    UINT8 num_bis;
    UINT8 nse;
    UINT16 iso_interval;
    UINT8 bn;
    UINT8 pto;
    UINT8 irc;
    UINT16 max_pdu;
    UINT32 sdu_interval;
    UINT16 max_sdu;
    UCHAR phy;
    UCHAR framing;
    UCHAR encryption;
} BAP_BC_SOURCE_STREAM_PARAMS;

typedef struct _BAP_BC_ASSOCIATE_PARAMS
{
    UINT8 error_code;
    UINT16 sync_handle;
    UINT8 advertising_sid;
    UINT8 advertiser_addr_type;
    UCHAR * advertiser_addr;
    UINT8 advertiser_phy;
    UINT16 periodic_adv_interval;
    UINT8 advertiser_clock_accuracy;
} BAP_BC_ASSOCIATE_PARAMS;

typedef struct _BAP_BC_DISSOCIATE_PARAMS
{
    UINT16 sync_handle;
} BAP_BC_DISSOCIATE_PARAMS;

typedef struct _BAP_BC_ENABLE_PARAMS
{
    UINT8 error_code;
    UINT8 big_handle;
    UINT32 transport_latency_big;
    UINT8 nse;
    UINT8 bn;
    UINT8 pto;
    UINT8 irc;
    UINT16 max_pdu;
    UINT16 iso_interval;
    UINT8 num_bis;
    UINT16 * connection_handle;
} BAP_BC_ENABLE_PARAMS;

typedef struct _BAP_BC_DISABLE_PARAMS
{
    UINT8 big_handle;
    UINT8 reason;
} BAP_BC_DISABLE_PARAMS;

typedef struct _BAP_BROADCAST_ANNOUNCEMENT
{
    UINT32 broadcast_id;

} BAP_BROADCAST_ANNOUNCEMENT;

typedef struct _BAP_BASIC_ANNOUNCEMENT_LEVEL_1
{
    GA_CODEC_INFO codec;
    GA_METADATA   meta;
} BAP_BASIC_ANNOUNCEMENT_LEVEL_1;

typedef struct _BAP_BASIC_ANNOUNCEMENT_LEVEL_2
{
    UINT8       stream_index;
    GA_CODEC_IE stream;
} BAP_BASIC_ANNOUNCEMENT_LEVEL_2;

/* --------------------------------------------- Macros */
#define BAP_GET_ASE_ID_FROM_BAP_ASE_INDEX(bap_ase_index, ase_id) \
    (*ase_id) = (bap_ase_index) + 1;

#define BAP_GET_BAP_ASE_INDEX_FROM_ASE_ID(ase_id, bap_ase_id) \
    (*bap_ase_id) = (ase_id) - 1;

#define BAP_ISEVENT_BROADCAST(ev) GA_BRR_ISEVENT_CONNECTIONLESS(ev)

/* --------------------------------------------- APIs */
GA_RESULT BAP_client_init(void);
GA_RESULT BAP_client_shutdown(void);
GA_RESULT BAP_server_init(void);
GA_RESULT BAP_server_shutdown(void);
GA_RESULT BAP_bc_source_init(void);
GA_RESULT BAP_bc_source_shutdown(void);
GA_RESULT BAP_bc_sink_init(void);
GA_RESULT BAP_bc_sink_shutdown(void);

GA_RESULT BAP_register_pac_record
          (
              /* IN */ UINT8 role,
              /* IN */ BAP_PAC_RECORD * record,
              /* OUT */ UINT8 * record_id
          );

GA_RESULT BAP_register_audio_contexts_and_locations
          (
              /* IN */ UINT8  role,
              /* IN */ UINT16 avail_audio_contexts,
              /* IN */ UINT16 supp_audio_contexts,
              /* IN */ UINT32 locations
          );

GA_RESULT BAP_update_and_notify_source_capabilities
          (
              /* IN */ GA_ENDPOINT * device,
              /* IN */ UINT16 capability,
              /* IN */ UINT8 id,
              /* IN */ void * arg,
              /* IN */ UINT16 size
          );

GA_RESULT BAP_update_and_notify_sink_capabilities
          (
              /* IN */ GA_ENDPOINT * device,
              /* IN */ UINT16 capability,
              /* IN */ UINT8 id,
              /* IN */ void * arg,
              /* IN */ UINT16 size
          );

#ifdef BAP_PACS_DEREGISTER
GA_RESULT BAP_deregister_pac_record
          (
              /* IN */ UINT8 role,
              /* IN */ UINT8 record_id
          );
#endif /* BAP_PACS_DEREGISTER */

GA_RESULT BAP_register_ase
          (
              /* IN */ UINT8    state,
              /* IN */ UINT8    role,
              /* OUT */ UINT8 * ase_id
          );

GA_RESULT BAP_setup_context
          (
              /* IN */  GA_ENDPOINT * device,
              /* OUT */ BAP_HANDLE * ci
          );

GA_RESULT BAP_release_context
          (
              /* IN */ BAP_HANDLE ci,
              /* IN */ UCHAR free
          );

GA_RESULT BAP_get_capability
          (
              /* IN */ BAP_HANDLE ci,
              /* IN */ UINT8 role
          );

GA_RESULT BAP_get_location
          (
              /* IN */ BAP_HANDLE ci,
              /* IN */ UINT8 role
          );

GA_RESULT BAP_get_supported_contexts
          (
              /* IN */ BAP_HANDLE ci
          );

GA_RESULT BAP_get_available_contexts
          (
              /* IN */ BAP_HANDLE ci
          );

GA_RESULT BAP_discover_ase
          (
              /* IN */ BAP_HANDLE ci
          );

GA_RESULT BAP_get_ase_status
          (
              /* IN */ BAP_HANDLE ci,
              /* IN */ UINT8 role,
              /* IN */ UINT8 se_index
          );

GA_RESULT BAP_ase_notify
          (
              GA_ENDPOINT * device,
              UINT8 role,
              UINT8 se_index,
              UCHAR * data,
              UINT16 length
          );

GA_RESULT BAP_asecp_request
          (
              /* IN */ BAP_HANDLE ci,
              /* IN */ UINT8 * se_index,
              /* IN */ UINT8 se_count,
              /* IN */ UINT8 operation,
              /* IN */ void * se_data,
              /* IN */ UINT16 se_datalen
          );

GA_RESULT BAP_asecp_response
          (
              /* IN */ GA_ENDPOINT * device,
              /* IN */ UINT8 * se_index,
              /* IN */ UINT8 se_count,
              /* IN */ UINT8 operation,
              /* IN */ UINT8 * response,
              /* IN */ UINT8 * reason
          );

#define BAP_asecp_configure_codec(ci, si, si_count, se_data, se_datalen) \
        BAP_asecp_request((ci), (si), (si_count), BAP_ASECP_CONFIG_CODEC, (se_data), (se_datalen))

#define BAP_asecp_configure_qos(ci, si, si_count, se_data, se_datalen) \
        BAP_asecp_request((ci), (si), (si_count), BAP_ASECP_CONFIG_QOS, (se_data), (se_datalen))

#define BAP_asecp_enable(ci, si, si_count, se_data, se_datalen) \
        BAP_asecp_request((ci), (si), (si_count), BAP_ASECP_ENABLE, (se_data), (se_datalen))

#define BAP_asecp_receiver_start_ready(ci, si, si_count) \
        BAP_asecp_request((ci), (si), (si_count), BAP_ASECP_RECEIVER_START_READY, NULL, 0)

#define BAP_asecp_update(ci, si, si_count, se_data, se_datalen) \
        BAP_asecp_request((ci), (si), (si_count), BAP_ASECP_UPDATE_METADATA, (se_data), (se_datalen))

#define BAP_asecp_disable(ci, si, si_count) \
        BAP_asecp_request((ci), (si), (si_count), BAP_ASECP_DISABLE, NULL, 0)

#define BAP_asecp_receiver_stop_ready(ci, si, si_count) \
        BAP_asecp_request((ci), (si), (si_count), BAP_ASECP_RECEIVER_STOP_READY, NULL, 0)

#define BAP_asecp_release(ci, si, si_count) \
        BAP_asecp_request((ci), (si), (si_count), BAP_ASECP_RELEASE, NULL, 0)

GA_RESULT BAP_broadcast_alloc_session
          (
              /* OUT */ UINT8 * ssn
          );

GA_RESULT BAP_broadcast_free_session
          (
              /* IN */ UINT8 ssn
          );

GA_RESULT BAP_broadcast_configure_session
          (
              /* IN */ UINT8 ssn,
              /* IN */ UINT8 phy,
              /* IN */ UINT8 packing,
              /* IN */ UCHAR * broadcast_code
          );

GA_RESULT BAP_broadcast_register_sep
          (
              /* IN */ UINT8 ssn,
              /* IN */ GA_CODEC_INFO * codec,
              /* IN */ GA_METADATA * meta,
              /* IN */ UINT8 nstream,
              /* IN */ GA_CODEC_IE * stream,
              /* OUT */ UINT8 * sep
          );

GA_RESULT BAP_broadcast_register_advset_info
          (
              /* IN */ UINT8 ssn,
              /* IN */ UINT8 advset_handle,
              /* IN */ UCHAR * sdata,
              /* IN */ UINT16  sdatalen,
              /* IN */ UCHAR * adata,
              /* IN */ UINT16  adatalen
          );

GA_RESULT BAP_broadcast_setup_announcement
          (
              /* IN */ UINT8 ssn,
              /* IN */ UINT32 broadcast_id,
              /* IN */ UINT8 type,
              /* IN */ UINT32 presentation_delay
          );

GA_RESULT BAP_broadcast_end_announcement
          (
              /* IN */ UINT8 ssn
          );

GA_RESULT BAP_broadcast_start
          (
              /* IN */ UINT8 ssn,
              /* IN */ UINT32 sdu_interval,
              /* IN */ UINT16 max_sdu,
              /* IN */ UINT16 max_latency,
              /* IN */ UINT8 rtn,
              /* IN */ UINT8 framing
          );

GA_RESULT BAP_broadcast_suspend
          (
              /* IN */ UINT8 ssn
          );

GA_RESULT BAP_broadcast_scan_announcement(void);

GA_RESULT BAP_broadcast_scan_end(void);

GA_RESULT BAP_broadcast_associate
          (
              UINT8 sid,
              GA_ENDPOINT * device
          );

GA_RESULT BAP_update_associate_info
          (
              UINT8 sid,
              GA_ENDPOINT * device,
              UINT16 sync_handle
          );

GA_RESULT BAP_broadcast_dissociate
          (
              UINT16 sync_handle
          );

GA_RESULT BAP_broadcast_enable
          (
              UINT16 sync_handle,
              UCHAR * broadcast_code,
              UINT8 bis_count,
              UINT8 * bis
          );

GA_RESULT BAP_broadcast_disable
          (
              UINT16 sync_handle
          );

GA_RESULT BAP_broadcast_decode_event_params
          (
              /* IN */  UINT8 event_type,
              /* IN */  UCHAR * event_data,
              /* OUT */ void * param
          );

GA_RESULT BAP_broadcast_decode_broadcast_announcement
          (
              /* IN */  UCHAR* data,
              /* IN */  UINT16 data_len,
              /* OUT */ BAP_BROADCAST_ANNOUNCEMENT * param
          );

GA_RESULT BAP_broadcast_decode_basic_announcement_info
          (
              /* IN */  UCHAR  * data,
              /* OUT */ UINT32 * presentation_delay,
              /* OUT */ UINT8  * n_sg,
              /* OUT */ UINT8  * parsed_len
          );

GA_RESULT BAP_broadcast_decode_basic_announcement_level_1
          (
              /* IN */  UCHAR                          * data,
              /* OUT */ BAP_BASIC_ANNOUNCEMENT_LEVEL_1 * param,
              /* OUT */ UINT8                          * n_bis,
              /* OUT */ UINT8                          * parsed_len
          );

GA_RESULT BAP_broadcast_decode_basic_announcement_level_2
          (
              /* IN */  UCHAR                          * data,
              /* OUT */ BAP_BASIC_ANNOUNCEMENT_LEVEL_2 * param,
              /* OUT */ UINT8                          * parsed_len
          );

GA_RESULT BAP_update_ntf_configuration(UINT32 config);

GA_RESULT BAP_manage_context_info
          (
              /* IN     */ UINT8              set,
              /* IN     */ GA_ENDPOINT      * device,
              /* INOUT  */ BAP_HANDLE       * handle,
              /* INOUT  */ GA_BRR_SVC_RANGE ** range,
              /* INOUT  */ GA_CHAR_INFO     ** info,
              /* INOUT  */ UINT16           * info_count
          );

#endif /* _H_BAP_ */

