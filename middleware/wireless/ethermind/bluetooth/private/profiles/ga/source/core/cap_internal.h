
/**
 *  \file cap_internal.h
 *
 *  \brief This file defines the internal data structures, function declarations,
 *         macros and constants used in CAP.
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_CAP_INTERNAL_
#define _H_CAP_INTERNAL_


/* --------------------------------------------- Header File Inclusion */
#include "cap.h"

/* --------------------------------------------- Debug Mapping */
#ifndef CAP_NO_DEBUG
#define CAP_ERR(...)                                GA_debug_error(GA_MODULE_ID_CAP, __VA_ARGS__)
#else  /* CAP_NO_DEBUG */
#define CAP_ERR                                     GA_debug_null
#endif /* CAP_NO_DEBUG */

#ifdef CAP_DEBUG

#define CAP_TRC(...)                                GA_debug_trace(GA_MODULE_ID_CAP, __VA_ARGS__)
#define CAP_INF(...)                                GA_debug_info(GA_MODULE_ID_CAP, __VA_ARGS__)

#define CAP_debug_dump_bytes(data, datalen) GA_debug_dump_bytes(GA_MODULE_ID_CAP, (data), (datalen))

#else /* CAP_DEBUG */

#define CAP_TRC                                     GA_debug_null
#define CAP_INF                                     GA_debug_null

#define CAP_debug_dump_bytes(data, datalen)

#endif /* CAP_DEBUG */

/* --------------------------------------------- Global Definitions */
/*
 *  This parameter defines the maximum number of stream contexts that
 *  can be setup by the application. There can be multiple stream contexts
 *  setup per connected peer device. The stream context is allocated
 *  during the ASE discovery procedure.
 */
#define GA_MAX_STREAM_CONTEXTS \
   (GA_CONFIG_LIMITS(GA_MAX_SOURCE_STREAM_CONTEXTS) + GA_CONFIG_LIMITS(GA_MAX_SINK_STREAM_CONTEXTS))

/** CAP Context States */
#define CAP_STATE_INITIALIZED               0x00U
#define CAP_STATE_IDLE                      0x01U
#define CAP_STATE_SETUP                     0x02U
#define CAP_STATE_SEP_DISCOVER              0x03U
#define CAP_STATE_CAPABILITIES              0x04U
#define CAP_STATE_CP_CONFIGURE              0x05U
#define CAP_STATE_RELEASE                   0x06U

/* Context LTV length defines */
#define GA_CONTEXT_LEN                         3U

/** CAP Context States */
#define CAP_VCP_STATE_INITIALIZED               0x00U
#define CAP_VCP_STATE_IDLE                      0x01U
#define CAP_VCP_STATE_IN_SETUP                  0x02U
#define CAP_VCP_STATE_IN_CH_RD                  0x03U
#define CAP_VCP_STATE_IN_CP_WT                  0x04U
#define CAP_VCP_STATE_GET_CAPABILITIES          0x05U
#define CAP_VCP_STATE_SET_CAPABILITY            0x06U
#define CAP_VCP_STATE_IN_RELEASE                0x07U
#define CAP_VCP_STATE_INVALID                   0xFFU

/** CAP Context States */
#define CAP_MICP_STATE_INITIALIZED               0x00U
#define CAP_MICP_STATE_IDLE                      0x01U
#define CAP_MICP_STATE_IN_SETUP                  0x02U
#define CAP_MICP_STATE_IN_CH_RD                  0x03U
#define CAP_MICP_STATE_IN_CP_WT                  0x04U
#define CAP_MICP_STATE_IN_GET_CAPABILITY         0x05U
#define CAP_MICP_STATE_IN_CH_WT                  0x06U
#define CAP_MICP_STATE_IN_RELEASE                0x07U
#define CAP_MICP_STATE_IN_SET_CAPABILITY         0x08U
#define CAP_MICP_STATE_INVALID                   0xFFU

#define CAP_CSIP_STATE_INITIALIZED               0x00U
#define CAP_CSIP_STATE_IDLE                      0x01U
#define CAP_CSIP_STATE_IN_SETUP                  0x02U
#define CAP_CSIP_STATE_IN_RELEASE                0x03U

/* --------------------------------------------- Data types /Structures */
/** State event strucutre */
typedef struct _CAP_STATE_EVENT
{
    /** State */
    UINT8 state;

    /** Event expected */
    UINT8 cevent;

} CAP_STATE_EVENT;

/** Stream Endpoint Capability */
typedef struct _CAP_STREAM_ENDPOINT
{
    /* Local Role */
    UINT8 sep_role;

    /* Endpoint ID */
    UINT8 ase_id;

    /* State */
    UINT8 state;

    /* Codec Configuration */
    GA_CODEC_INFO codec;

    /* Cached Codec Configuration when locally registered */
    GA_CODEC_INFO codec_cache;

    /* QOS Capability */
    GA_QOS_CAP qoscap;

    /* QOS Configuration */
    GA_QOS_CONF qosconf;

    /* Audio Channel reference */
    GA_CHANNEL channel;

    /* Metadata information */
    GA_METADATA meta;

    /* Mark Pend Notification */
    /* TODO: Has to be per device basis */
    UINT8 pend_notify;

} CAP_STREAM_ENDPOINT;

/** GA core context */
typedef struct _CAP_CONTEXT
{
    /* Endpoint */
    GA_ENDPOINT device;

#ifdef GA_BAP
    /* State */
    UINT8 state;

    /* Event for operation */
    UINT8 cevent;

    /* Peer role in current context of operation */
    UINT8 role;

    /* ASE count - Source */
    UINT8 src_sep_count;

    /* ASE count - Sink */
    UINT8 snk_sep_count;

    /* Current ASE index */
    UINT8 sep_index;

    /* Current ASE index Role */
    UINT8 sep_role;

    /* BAP Profile ID */
    BAP_HANDLE bap_id;
#endif /* GA_BAP */

#ifdef GA_VCP
    /* VCP Profile ID */
    VCP_HANDLE vcp_id;

    /* VCP State Event structure */
    CAP_STATE_EVENT vcp_se;
#endif /* GA_VCP */

#ifdef GA_MICP
    /* MICP Profile ID */
    MICP_HANDLE micp_id;

    /* MICP State Event structure */
    CAP_STATE_EVENT micp_se;
#endif /* GA_MICP */

#ifdef GA_CSIP
    /* CSIP Profile ID */
    CSIP_HANDLE csip_id;

    /* CSIP State Event structure */
    CAP_STATE_EVENT csip_se;
#endif /* GA_CSIP */
} CAP_CONTEXT;

/** GA Stream Contexts */
typedef struct _CAP_STREAM_CONTEXT
{
    /* Stream endpoint ID */
    UINT8 sep_id;

    /* CAP Context ID Associated to this Endpoint */
    UINT8 cap_id;

    /* ASE ID */
    UINT8 ase_id;

    /* ASE state */
    UINT8 ase_state;

    /* SEP Event type for synchronous CP operations */
    UINT8 sep_event;
} CAP_STREAM_CONTEXT;

/** CAP CSIP Session */
typedef struct _CAP_CSIP_SESSION
{
    /* Address of Coordinator handling the session */
    GA_ENDPOINT sc_addr;

    /* Current lock state for the session */
    UINT8 sc_lock;

    /* Session lock timer - TODO */
    GA_timer_handle sc_timer;

} CAP_CSIP_SESSION;

/** SEP Control Point Handler */
typedef GA_RESULT (* CAP_SEP_CP_HANDLER)
                  (
                      GA_ENDPOINT * device,
                      UINT8 count,
                      UCHAR * data,
                      UINT16 datalen
                  );

/* --------------------------------------------- Macros */
/* Lock and Unlock Macros */
#define CAP_LOCK()                  GA_MUTEX_LOCK(cap_mutex, CAP)
#define CAP_LOCK_VOID()             GA_MUTEX_LOCK_VOID(cap_mutex, CAP)
#define CAP_UNLOCK()                GA_MUTEX_UNLOCK(cap_mutex, CAP)
#define CAP_UNLOCK_VOID()           GA_MUTEX_UNLOCK_VOID(cap_mutex, CAP)

/* --------------------------------------------- Functions */
GA_RESULT ga_brr_callback
          (
              /* IN */ GA_BRR_DEVICE * device,
              /* IN */ UINT8 event_type,
              /* IN */ UINT16 event_status,
              /* IN */ void * event_data,
              /* IN */ UINT16 event_datalen
          );
GA_RESULT cap_service_callback
          (
              /* IN */ GA_BRR_DEVICE * device,
              /* IN */ void * context,
              /* IN */ UINT8 event_type,
              /* IN */ UINT16 event_status,
              /* IN */ void * event_data,
              /* IN */ UINT16 event_datalen
          );
GA_RESULT cap_profile_callback
          (
              /* IN */ GA_BRR_DEVICE * device,
              /* IN */ void * context,
              /* IN */ UINT8 event_type,
              /* IN */ UINT16 event_status,
              /* IN */ void * event_data,
              /* IN */ UINT16 event_datalen
          );

CAP_CONTEXT * cap_alloc_context
              (
                  /* IN  */  GA_ENDPOINT    * device,
                  /* OUT */  UINT8    *ci
              );
void cap_free_context(CAP_CONTEXT * ctx);

CAP_CONTEXT * cap_get_context(GA_CONTEXT * context, UINT8 state);
CAP_CONTEXT * cap_search_context(GA_ENDPOINT * device, UINT8 * ci);

#ifdef GA_BAP
GA_RESULT cap_init_bap_unicast(UINT8 role, GA_PROFILE_CB cb);
GA_RESULT cap_shutdown_bap_unicast(UINT8 role);
void cap_create_preferred_contexts_ltv
     (
         UINT16 contexts,
         UCHAR * ltvarray,
         UINT8 * ltvarray_len
     );
void cap_create_content_ltv
     (
         GA_CONTENT_INFO * content,
         UCHAR * ltvarray,
         UINT8 * ltvarray_len
     );
UINT8 cap_alloc_stream_context
      (
          UINT8 ci,
          UINT8 sep_id,
          UINT8 ase_id,
          UINT8 ase_state
      );
UINT8 cap_find_stream_context(UINT8 ci, UINT8 ase_id);
CAP_STREAM_ENDPOINT * cap_find_sep(UINT8 ase_id, UINT8 * si);
GA_RESULT cap_handle_asecp_codec_configure
          (
              GA_ENDPOINT * device,
              UINT8 count,
              UCHAR * data,
              UINT16 datalen
          );
GA_RESULT cap_handle_asecp_qos_configure
          (
              GA_ENDPOINT * device,
              UINT8 count,
              UCHAR * data,
              UINT16 datalen
          );
GA_RESULT cap_handle_asecp_enable
          (
              GA_ENDPOINT * device,
              UINT8 count,
              UCHAR * data,
              UINT16 datalen
          );
GA_RESULT cap_handle_asecp_rx_start_ready
          (
              GA_ENDPOINT * device,
              UINT8 count,
              UCHAR * data,
              UINT16 datalen
          );
GA_RESULT cap_handle_asecp_update_metadata
          (
              GA_ENDPOINT * device,
              UINT8 count,
              UCHAR * data,
              UINT16 datalen
          );
GA_RESULT cap_handle_asecp_rx_stop_ready
          (
              GA_ENDPOINT * device,
              UINT8 count,
              UCHAR * data,
              UINT16 datalen
          );
GA_RESULT cap_handle_asecp_disable
          (
              GA_ENDPOINT * device,
              UINT8 count,
              UCHAR * data,
              UINT16 datalen
          );
GA_RESULT cap_handle_asecp_release
          (
              GA_ENDPOINT * device,
              UINT8 count,
              UCHAR * data,
              UINT16 datalen
          );

GA_RESULT gas_callback
          (
              GA_ENDPOINT * device,
              UINT8 event_type,
              UINT16 event_status,
              void * event_data,
              UINT16 event_datalen
          );

GA_RESULT gac_callback
          (
              GA_ENDPOINT * device,
              UINT8 event_type,
              UINT16 event_status,
              void * event_data,
              UINT16 event_datalen
          );

GA_RESULT sep_notify_updates(GA_ENDPOINT * device);

#ifdef BAP_BROADCAST
GA_RESULT cap_init_bap_broadcast(UINT8 role, GA_PROFILE_CB cb);
GA_RESULT cap_shutdown_bap_broadcast(UINT8 role);
GA_RESULT cap_process_bap_bc_event
          (
              GA_ENDPOINT * device,
              UINT8 event_type,
              UINT16 status,
              void * data,
              UINT16 datalen
          );
#endif /* BAP_BROADCAST */
#endif /* GA_BAP */

#ifdef GA_VCP
GA_RESULT cap_init_vcp(UINT8 role, GA_PROFILE_CB cb);
GA_RESULT cap_shutdown_vcp(UINT8 role);
CAP_CONTEXT * cap_search_vcp_context(GA_VC_CONTEXT * vctx, UINT8 * ci);
#endif /* GA_VCP */

#ifdef GA_MICP
GA_RESULT cap_init_micp(UINT8 role, GA_PROFILE_CB cb);
GA_RESULT cap_shutdown_micp(UINT8 role);
CAP_CONTEXT * cap_search_micp_context(GA_MC_CONTEXT * mctx, UINT8 * ci);
#endif /* GA_MICP */

#ifdef GA_CSIP
GA_RESULT cap_init_csip(UINT8 role, GA_PROFILE_CB cb);
GA_RESULT cap_shutdown_csip(UINT8 role);
CAP_CONTEXT * cap_search_csip_context(CSIP_HANDLE * handle, UINT8 * ci);
void cap_csm_lock_timeout_handler(void * args, UINT16 size);
UINT16 cap_handle_csip_lock(GA_ENDPOINT * device, UINT8 lock);
#endif /* CSIP */
#endif /* _H_CAP_INTERNAL_ */

