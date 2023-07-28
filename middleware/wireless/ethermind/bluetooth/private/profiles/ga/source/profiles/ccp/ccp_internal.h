
/**
 *  \file ccp_internal.h
 *
 *  \brief This file internal components of the Call Control Profile.
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_CCP_INTERNAL_
#define _H_CCP_INTERNAL_

/* --------------------------------------------- Header File Inclusion */
#include "GA_ccp_ce_api.h"
#include "GA_ccp_se_api.h"

/* --------------------------------------------- Debug Mapping */
#ifndef CCP_NO_DEBUG
#define CCP_ERR(...)                                GA_debug_error(GA_MODULE_ID_CCP, __VA_ARGS__)
#else  /* CCP_NO_DEBUG */
#define CCP_ERR                                     GA_debug_null
#endif /* CCP_NO_DEBUG */

#ifdef CCP_DEBUG

#define CCP_TRC(...)                                GA_debug_trace(GA_MODULE_ID_CCP, __VA_ARGS__)
#define CCP_INF(...)                                GA_debug_info(GA_MODULE_ID_CCP, __VA_ARGS__)

#define CCP_debug_dump_bytes(data, datalen) GA_debug_dump_bytes(GA_MODULE_ID_CCP, (data), (datalen))

#else /* CCP_DEBUG */

#define CCP_TRC                                     GA_debug_null
#define CCP_INF                                     GA_debug_null

#define CCP_debug_dump_bytes(data, datalen)

#endif /* CCP_DEBUG */

/* --------------------------------------------- Global Definitions */
#define CCP_INVALID_CNTX_ID               0xFFU
#define CCP_INVALID_EVENT                 0xFFU
#define CCP_SVC_TYPE_INVALID              0xFFU

/* --------------------------------------------- Data types /Structures */

#ifdef CCP_CLIENT
/**
 * GTBS/TBS Information
 */
typedef struct  _CCP_TBS_INFO
{
    /**
     * Service Range
     */
    GA_BRR_SVC_RANGE range;

    /**
     * Characteristic Information
     */
    GA_CHAR_INFO char_info[CCP_CHAR_ID_COUNT];

    /**
     * Current characteristic ID being configured
     */
    UINT8 char_config_id;

} CCP_TBS_INFO;

/**
 * Data Structure for CCP Client entity
 */
typedef struct  _CCP_CE
{
    /* Endpoint */
    GA_ENDPOINT device;

    /**
     * Pointer to GTBS Char Handles
     */
    CCP_TBS_INFO gtbs;

    /**
     * Context id
     * - To indicate if allocated
     */
    CCP_HANDLE ctx_id;

    /* Context Event for a procedure */
    UINT8 cntx_event;

} CCP_CE;


typedef struct  _CCP_CE_TBS
{
    /* Endpoint reference */
    UINT8 ccp_id;

    /* Pointer to TBS char handles */
    CCP_TBS_INFO tbs;

} CCP_CE_TBS;

#endif /* CCP_CLIENT  */


#ifdef CCP_SERVER
/**
 * Data Structure for TBS Server Entity
 */
typedef struct  _CCP_TBS_ENTITY
{
    /**
     * Context id
     * - To indicate if allocated
     */
    UCHAR   ctx_id;

    /**
     *  Service Info.
     */
    GA_BRR_SVC_INFO svc_info;

    /* Service Instance Id */
    GA_BRR_SVC_INST tbs_id;

    /**
     * Bearer Provider Name
     */
    GA_BRR_CHR_INST char_id_BEARER_PROVIDER_NAME;

    /**
     * Bearer Uniform Call Identifier (UCI)
     */
    GA_BRR_CHR_INST char_id_BEARER_UNIFORM_CALL_IDENTIFIER;

    /**
     * Bearer Technology
     */
    GA_BRR_CHR_INST char_id_BEARER_TECHNOLOGY;

    /**
     * Bearer URI Schemes Supported List
     */
    GA_BRR_CHR_INST char_id_BEARER_URI_SCHEMES_SUPPORTED_LIST;

    /**
     * Bearer Signal Strength
     */
    GA_BRR_CHR_INST char_id_BEARER_SIGNAL_STRENGTH;

    /**
     * Bearer Signal Strength Reporting Interval
     */
    GA_BRR_CHR_INST char_id_BEARER_SIGNAL_STRGTH_RPRT_INTRVAL;

    /**
     * Bearer List Current Calls
     */
    GA_BRR_CHR_INST char_id_BEARER_LIST_CURRENT_CALLS;

    /**
     * Content Control ID
     */
    GA_BRR_CHR_INST char_id_CONTENT_CONTROL_ID;

    /**
     * Status Flags
     */
    GA_BRR_CHR_INST char_id_STATUS_FLAGS;

    /**
     * Incoming Call Target Bearer URI
     */
    GA_BRR_CHR_INST char_id_INCOMING_CALL_TRGT_BRR_URI;

    /**
     * Call State
     */
    GA_BRR_CHR_INST char_id_CALL_STATE;

    /**
     * Call Control Point
     */
    GA_BRR_CHR_INST char_id_CALL_CP;

    /**
     * Call Control Point Optional Opcodes
     */
    GA_BRR_CHR_INST char_id_CALL_CP_OPTIONAL_OPCODES;

    /**
     * Termination Reason
     */
    GA_BRR_CHR_INST char_id_TERMINATION_REASON;

    /**
     * Incoming Call
     */
    GA_BRR_CHR_INST char_id_INCOMING_CALL;

    /**
     * Call Friendly Name
     */
    GA_BRR_CHR_INST char_id_CALL_FRIENDLY_NAME;
}CCP_TBS_ENTITY;

#endif /* CCP_SERVER */

/* --------------------------------------------- Macros */
#ifdef CCP_CLIENT
/** CCP Client  Mutex Lock & Unlock */
#define CCP_CE_LOCK()                   GA_MUTEX_LOCK(ccp_ce_mutex, CCP)
#define CCP_CE_LOCK_VOID()              GA_MUTEX_LOCK_VOID(ccp_ce_mutex, CCP)
#define CCP_CE_UNLOCK()                 GA_MUTEX_UNLOCK(ccp_ce_mutex, CCP)
#define CCP_CE_UNLOCK_VOID()            GA_MUTEX_UNLOCK_VOID(ccp_ce_mutex, CCP)
#endif /* CCP_CLIENT */

#ifdef CCP_SERVER
/** CCP Server  Mutex Lock & Unlock */
#define CCP_SE_LOCK()                   GA_MUTEX_LOCK(ccp_se_mutex, CCP)
#define CCP_SE_LOCK_VOID()              GA_MUTEX_LOCK_VOID(ccp_se_mutex, CCP)
#define CCP_SE_UNLOCK()                 GA_MUTEX_UNLOCK(ccp_se_mutex, CCP)
#define CCP_SE_UNLOCK_VOID()            GA_MUTEX_UNLOCK_VOID(ccp_se_mutex, CCP)
#endif /* CCP_SERVER */

/* --------------------------------------------- APIs */

#ifdef CCP_CLIENT
GA_RESULT ccp_ce_discover
          (
              /* IN */ GA_ENDPOINT * device,
              /* IN */ UINT16 service,
              /* OUT */ CCP_HANDLE  * handle
          );

CCP_CE * ccp_ce_search_entity(GA_ENDPOINT * device, UCHAR * handle);

CCP_CE_TBS * ccp_ce_search_tbs_entity
             (
                 /* IN  */ GA_ENDPOINT      * device,
                 /* IN  */ GA_BRR_SVC_RANGE * range,
                 /* OUT */ UCHAR            * tbs_id
             );

CCP_CE * ccp_ce_alloc_entity(GA_ENDPOINT * device, UCHAR * handle);

CCP_CE_TBS * ccp_ce_alloc_tbs_entity(UCHAR * handle);

void ccp_ce_free_entity(UCHAR handle);

void ccp_ce_free_context(UCHAR handle);

GA_RESULT ccp_ce_reference_context
          (
              /* IN  */ void   * ctx,
              /* OUT */ UCHAR  * handle,
              /* OUT */ UCHAR  * svc
          );

GA_RESULT ccp_ce_reference_char_info
          (
              /* IN  */ GA_BRR_CHR_HANDLE chr,
              /* OUT */ UCHAR  * handle,
              /* OUT */ UCHAR  * svc
          );

void ccp_ce_free_tbs_entity(UCHAR handle);

GA_RESULT ccp_ce_release_context
          (
              /* IN */ UCHAR  cntx_idx,
              /* IN */ UCHAR  srvc_type
          );

GA_RESULT ccp_ce_get_ntf_event_from_char_handle
          (
              /* IN  */ UINT16  chr_handle,
              /* IN */  CCP_TBS_INFO *tbs,
              /* OUT */ UINT8   *ccp_ntf_event
          );

void ccp_ce_handle_composition
     (
         /* IN */ UINT8 ci,
         /* IN */ UCHAR svc_type,
         /* IN */ GA_BRR_SERVICE_COMPOSITION * comp
     );

void ccp_ce_handle_char_composition
     (
         /* IN */ UINT8 ci,
         /* IN */ UCHAR svc_type,
         /* IN */ GA_BRR_CHARACTERISTIC_COMPOSITION * comp
     );


GA_RESULT ccp_ce_profile_callback
          (
              /* IN */ GA_BRR_DEVICE * device,
              /* IN */ void * context,
              /* IN */ UINT8 event_type,
              /* IN */ UINT16 event_status,
              /* IN */ void * event_data,
              /* IN */ UINT16 event_datalen
          );

/* Get Char Ref ID for this UUID */
UINT8 ccp_ce_get_char_id_from_uuid
      (
          /* IN */ UINT16 uuid
      );

/* Get Char Ref ID for this GTBS/TBS instance based on the Char Handle */
UINT8 ccp_ce_get_char_id_from_handle
      (
          /* IN */ UINT16 char_handle,
          /* IN */ CCP_TBS_INFO * tbs
      );

/* Get Next Char Ref ID for this GTBS/TBS instance based on the Char Ref ID */
UINT8 ccp_ce_get_next_char_config
      (
          /* IN */ UINT8 char_id,
          /* IN */ CCP_TBS_INFO * tbs
      );
/* Begin Notification Disable for this GTBS/TBS instance */
GA_RESULT ccp_ce_notification_disable
          (
              /* IN */  GA_BRR_DEVICE * device,
              /* IN */  void * ctx,
              /* IN */  CCP_TBS_INFO * tbs
          );
#endif /*  CCP_CLIENT */

#ifdef CCP_SERVER
/* ====== Server related ========= */
GA_RESULT ccp_se_service_callback
          (
              /* IN */ GA_BRR_DEVICE * device,
              /* IN */ void * context,
              /* IN */ UINT8 event_type,
              /* IN */ UINT16 event_status,
              /* IN */ void * event_data,
              /* IN */ UINT16 event_datalen
          );

GA_RESULT ccp_se_handle_read_request
          (
              /* IN */ GA_ENDPOINT * device,
              /* IN */ GA_BRR_CHR_CONTEXT * chr,
              /* IN */ UINT16 event_status,
              /* IN */ UCHAR * event_data,
              /* IN */ UINT16 event_datalen
          );

GA_RESULT ccp_se_handle_write_request
          (
              /* IN */ GA_ENDPOINT * device,
              /* IN */ GA_BRR_CHR_CONTEXT * chr,
              /* IN */ UINT16 event_status,
              /* IN */ UCHAR * event_data,
              /* IN */ UINT16 event_datalen
          );

void ccp_se_handle_config_request
     (
         /* IN */ GA_ENDPOINT * device,
         /* IN */ GA_BRR_CHR_CONTEXT * chr,
         /* IN */ UINT16 event_status,
         /* IN */ UCHAR * event_data,
         /* IN */ UINT16 event_datalen
     );

GA_RESULT ccp_se_get_srvs_id_from_char_id
          (
              /* OUT */ GA_BRR_SVC_INST *srvc_inst,
              /* IN  */ GA_BRR_CHR_INST  char_inst
          );

GA_RESULT ccp_se_get_char_id_from_char_uuid
          (
              /* IN */   GA_BRR_SVC_INST    srvc_inst,
              /* IN */   UINT16             char_uuid,
              /* OUT  */ GA_BRR_CHR_INST  *char_inst
          );

GA_RESULT  ccp_se_register_service
           (
               /* IN */  UINT16   srvs_uuid,
               /* OUT */ UCHAR   *srvs_handle
           );
#endif /* CCP_SERVER */

#endif /* _H_CCP_INTERNAL */



