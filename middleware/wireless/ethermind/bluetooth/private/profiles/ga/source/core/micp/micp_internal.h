
/**
 *  \file micp_internal.h
 *
 *  \brief This file defines the Volume Control Profile Interface
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_MICP_INTERNAL_
#define _H_MICP_INTERNAL_


/* --------------------------------------------- Header File Inclusion */
#include "micp.h"

/* --------------------------------------------- Debug Mapping */
#ifndef MICP_NO_DEBUG
#define MICP_ERR(...)                                GA_debug_error(GA_MODULE_ID_MICP, __VA_ARGS__)
#else  /* MICP_NO_DEBUG */
#define MICP_ERR                                     GA_debug_null
#endif /* MICP_NO_DEBUG */

#ifdef MICP_DEBUG

#define MICP_TRC(...)                                GA_debug_trace(GA_MODULE_ID_MICP, __VA_ARGS__)
#define MICP_INF(...)                                GA_debug_info(GA_MODULE_ID_MICP, __VA_ARGS__)

#define MICP_debug_dump_bytes(data, datalen) GA_debug_dump_bytes(GA_MODULE_ID_MICP, (data), (datalen))

#else /* MICP_DEBUG */

#define MICP_TRC                                     GA_debug_null
#define MICP_INF                                     GA_debug_null

#define MICP_debug_dump_bytes(data, datalen)

#endif /* MICP_DEBUG */

/* --------------------------------------------- Global Definitions */

/* --------------------------------------------- Data types /Structures */

/* --------------------------------------------- Macros */
/* Lock and Unlock macros for MICP */
#define MICC_LOCK()              GA_MUTEX_LOCK(micc_mutex, MICP)
#define MICC_LOCK_VOID()         GA_MUTEX_LOCK_VOID(micc_mutex, MICP)
#define MICC_UNLOCK()            GA_MUTEX_UNLOCK(micc_mutex, MICP)
#define MICC_UNLOCK_VOID()       GA_MUTEX_UNLOCK_VOID(micc_mutex, MICP)

#define MICS_LOCK()              GA_MUTEX_LOCK(mics_mutex, MICP)
#define MICS_LOCK_VOID()         GA_MUTEX_LOCK_VOID(mics_mutex, MICP)
#define MICS_UNLOCK()            GA_MUTEX_UNLOCK(mics_mutex, MICP)
#define MICS_UNLOCK_VOID()       GA_MUTEX_UNLOCK_VOID(mics_mutex, MICP)

/* --------------------------------------------- APIs */
void micp_add_aics(/* IN */ MICS_AICS_CONTEXT * aics_entity);

GA_RESULT micp_get_context
          (
              /* IN    */ void   *cntx_pointer,
              /* OUT   */ UCHAR  *cntx_idx,
              /* OUT   */ UCHAR  *srvc_type
          );

GA_RESULT micp_free_context
          (
              /* IN */ UCHAR  cntx_idx
          );

void micp_handle_composition
     (
         GA_BRR_SERVICE_COMPOSITION * comp,
         GA_CHAR_INFO * char_info,
         UINT8 charid_count,
         GA_CHAR_UUID * uuid
     );

void micp_handle_char_composition
     (
         GA_BRR_CHARACTERISTIC_COMPOSITION * comp,
         GA_CHAR_INFO * char_info,
         UINT8 charid_count,
         UINT8 char_config_id
     );

GA_RESULT micp_profile_callback
          (
              /* IN */ GA_BRR_DEVICE * device,
              /* IN */ void * context,
              /* IN */ UINT8 event_type,
              /* IN */ UINT16 event_status,
              /* IN */ void * event_data,
              /* IN */ UINT16 event_datalen
          );

GA_RESULT micp_service_callback
          (
              /* IN */ GA_BRR_DEVICE * device,
              /* IN */ void * context,
              /* IN */ UINT8 event_type,
              /* IN */ UINT16 event_status,
              /* IN */ void * event_data,
              /* IN */ UINT16 event_datalen
          );

void micp_handle_read_request
     (
         /* IN */ GA_ENDPOINT * device,
         /* IN */ GA_BRR_CHR_CONTEXT * chr,
         /* IN */ UINT16 event_status,
         /* IN */ UCHAR * event_data,
         /* IN */ UINT16 event_datalen
     );

void micp_handle_write_request
     (
         /* IN */ GA_ENDPOINT * device,
         /* IN */ GA_BRR_CHR_CONTEXT * chr,
         /* IN */ UINT16 event_status,
         /* IN */ UCHAR * event_data,
         /* IN */ UINT16 event_datalen
     );

void micp_handle_config_request
     (
         /* IN */ GA_ENDPOINT * device,
         /* IN */ GA_BRR_CHR_CONTEXT * chr,
         /* IN */ UINT16 event_status,
         /* IN */ UCHAR * event_data,
         /* IN */ UINT16 event_datalen
     );

GA_RESULT micp_release_context
          (
              /* IN */ UCHAR  cntx_idx,
              /* IN */ UCHAR  srvc_type
          );

GA_RESULT micp_get_aics_char_id_from_char_uuid
          (
              /* IN */  MICS_AICS_CONTEXT  *srvc_info,
              /* IN */  UINT16             char_uuid,
              /* OUT  */ GA_BRR_CHR_INST  *char_inst
          );

/* Get Service Id from the Char ID */
GA_RESULT mcd_get_srvc_id_from_char_id
          (
              /* OUT */ GA_BRR_SVC_INST *srvc_inst,
              /* OUT */ UCHAR           *srvc_type,
              /* IN  */ GA_BRR_CHR_INST  char_inst
          );

GA_RESULT micp_search_device_context
          (
              /* IN  */ GA_ENDPOINT * device,
              /* OUT */ UCHAR       * ci
          );

GA_RESULT micp_search_aics_context
          (
              /* IN */  MICP_HANDLE handle,
              /* IN */  GA_BRR_SVC_RANGE *range,
              /* OUT */ UCHAR    * ci
          );

UINT8 micp_get_char_id_from_uuid
      (
          /* IN */ UCHAR char_id_cnt,
          /* IN */ UINT16 uuid,
          /* IN */ GA_CHAR_UUID *srvs_char_uuid
      );

UINT8 micp_get_char_id_from_handle
      (
          /* IN */ UCHAR char_id_cnt,
          /* IN */ UINT16 char_handle,
          /* IN */ GA_CHAR_INFO *ga_char_info
      );

UINT8 micp_get_next_char_config
      (
          /* IN */ UCHAR  char_id_cnt,
          /* IN */ UINT8  char_id,
          /* IN */ UINT32  micp_srvs_config,
          /* IN */ GA_CHAR_INFO *ga_char_info
      );

#endif /* _H_MICP_INTERNAL */



