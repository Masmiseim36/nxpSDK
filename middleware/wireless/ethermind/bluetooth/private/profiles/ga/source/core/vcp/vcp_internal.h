
/**
 *  \file vcp_internal.h
 *
 *  \brief This file defines the Volume Control Profile Interface
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_VCP_INTERNAL_
#define _H_VCP_INTERNAL_


/* --------------------------------------------- Header File Inclusion */
#include "vcp.h"

/* --------------------------------------------- Debug Mapping */
#ifndef VCP_NO_DEBUG
#define VCP_ERR(...)                                GA_debug_error(GA_MODULE_ID_VCP, __VA_ARGS__)
#else  /* VCP_NO_DEBUG */
#define VCP_ERR                                     GA_debug_null
#endif /* VCP_NO_DEBUG */

#ifdef VCP_DEBUG

#define VCP_TRC(...)                                GA_debug_trace(GA_MODULE_ID_VCP, __VA_ARGS__)
#define VCP_INF(...)                                GA_debug_info(GA_MODULE_ID_VCP, __VA_ARGS__)

#define VCP_debug_dump_bytes(data, datalen) GA_debug_dump_bytes(GA_MODULE_ID_VCP, (data), (datalen))

#else /* VCP_DEBUG */

#define VCP_TRC                                     GA_debug_null
#define VCP_INF                                     GA_debug_null

#define VCP_debug_dump_bytes(data, datalen)

#endif /* VCP_DEBUG */

/* --------------------------------------------- Global Definitions */

/* --------------------------------------------- Data types /Structures */

/* --------------------------------------------- Macros */
/* Lock and Unlock macros for VCP */
#define VCC_LOCK()              GA_MUTEX_LOCK(vcc_mutex, VCP)
#define VCC_LOCK_VOID()         GA_MUTEX_LOCK_VOID(vcc_mutex, VCP)
#define VCC_UNLOCK()            GA_MUTEX_UNLOCK(vcc_mutex, VCP)
#define VCC_UNLOCK_VOID()       GA_MUTEX_UNLOCK_VOID(vcc_mutex, VCP)

#define VCS_LOCK()              GA_MUTEX_LOCK(vcs_mutex, VCP)
#define VCS_LOCK_VOID()         GA_MUTEX_LOCK_VOID(vcs_mutex, VCP)
#define VCS_UNLOCK()            GA_MUTEX_UNLOCK(vcs_mutex, VCP)
#define VCS_UNLOCK_VOID()       GA_MUTEX_UNLOCK_VOID(vcs_mutex, VCP)

/* --------------------------------------------- APIs */
void vcp_add_vocs(VCS_VOCS_ENTITY * vocs_entity);
void vcp_add_aics(VCS_AICS_ENTITY * aics_entity);

GA_RESULT vcp_get_context
          (
              /* IN    */ void   *cntx_pointer,
              /* OUT   */ UCHAR  *cntx_idx,
              /* OUT   */ UCHAR  *srvc_type
          );

GA_RESULT vcp_free_context
          (
              /* IN */ UCHAR  cntx_idx
          );

GA_RESULT vcp_release_context
          (
              /* IN */ UCHAR  cntx_idx,
              /* IN */ UCHAR  srvc_type
          );

void vcp_handle_composition
     (
         GA_BRR_SERVICE_COMPOSITION * comp,
         GA_CHAR_INFO * char_info,
         UINT8 charid_count,
         GA_CHAR_UUID * uuid
     );

void vcp_handle_char_composition
     (
         GA_BRR_CHARACTERISTIC_COMPOSITION * comp,
         GA_CHAR_INFO * char_info,
         UINT8 charid_count,
         UINT8 char_config_id
     );

GA_RESULT vcp_profile_callback
          (
              /* IN */ GA_BRR_DEVICE * device,
              /* IN */ void * context,
              /* IN */ UINT8 event_type,
              /* IN */ UINT16 event_status,
              /* IN */ void * event_data,
              /* IN */ UINT16 event_datalen
          );

GA_RESULT vcp_service_callback
          (
              /* IN */ GA_BRR_DEVICE * device,
              /* IN */ void * context,
              /* IN */ UINT8 event_type,
              /* IN */ UINT16 event_status,
              /* IN */ void * event_data,
              /* IN */ UINT16 event_datalen
          );

void vcp_handle_read_request
     (
         /* IN */ GA_ENDPOINT * device,
         /* IN */ GA_BRR_CHR_CONTEXT * chr,
         /* IN */ UINT16 event_status,
         /* IN */ UCHAR * event_data,
         /* IN */ UINT16 event_datalen
     );

void vcp_handle_write_request
     (
         /* IN */ GA_ENDPOINT * device,
         /* IN */ GA_BRR_CHR_CONTEXT * chr,
         /* IN */ UINT16 event_status,
         /* IN */ UCHAR * event_data,
         /* IN */ UINT16 event_datalen
     );

void vcp_handle_config_request
     (
         /* IN */ GA_ENDPOINT * device,
         /* IN */ GA_BRR_CHR_CONTEXT * chr,
         /* IN */ UINT16 event_status,
         /* IN */ UCHAR * event_data,
         /* IN */ UINT16 event_datalen
     );

GA_RESULT vcp_search_device_context
          (
              /* IN  */ GA_ENDPOINT * device,
              /* OUT */ UCHAR       * ci
          );

GA_RESULT vcp_search_vocs_context
          (
              /* IN */  VCP_HANDLE handle,
              /* IN */  GA_BRR_SVC_RANGE *range,
              /* OUT */ UCHAR    * ci
          );

GA_RESULT vcp_search_aics_context
          (
              /* IN */  VCP_HANDLE handle,
              /* IN */  GA_BRR_SVC_RANGE *range,
              /* OUT */ UCHAR    * ci
          );

GA_RESULT vcp_get_srvc_id_from_char_id
          (
              /* OUT */ GA_BRR_SVC_INST *srvc_inst,
              /* OUT */ UCHAR           *srvc_type,
              /* IN  */ GA_BRR_CHR_INST  char_inst
          );

GA_RESULT vcr_get_vcs_char_id_from_char_uuid
          (
              /* IN */  VCS_ENTTITY     *srvc_info,
              /* IN */  UINT16            char_uuid,
              /* OUT */ GA_BRR_CHR_INST  *char_inst
          );

GA_RESULT vcr_get_vocs_char_id_from_char_uuid
          (
              /* IN */  VCS_VOCS_ENTITY   *srvc_info,
              /* IN */  UINT16             char_uuid,
              /* OUT */ GA_BRR_CHR_INST   *char_inst
          );

GA_RESULT vcr_get_aics_char_id_from_char_uuid
          (
              /* IN */  VCS_AICS_ENTITY   *srvc_info,
              /* IN */  UINT16             char_uuid,
              /* OUT */ GA_BRR_CHR_INST  *char_inst
          );

UINT8 vcp_get_char_id_from_uuid
      (
          /* IN */ UCHAR char_id_cnt,
          /* IN */ UINT16 uuid,
          /* IN */ GA_CHAR_UUID *srvs_char_uuid
      );

UINT8 vcp_get_char_id_from_handle
      (
          /* IN */ UCHAR char_id_cnt,
          /* IN */ UINT16 char_handle,
          /* IN */ GA_CHAR_INFO *ga_char_info
      );

UINT8 vcp_get_next_char_config
      (
          /* IN */ UCHAR  char_id_cnt,
          /* IN */ UINT8  char_id,
          /* IN */ UINT32  vcp_srvs_config,
          /* IN */ GA_CHAR_INFO *ga_char_info
      );

UINT16 vcp_get_next_char_handle_for_config
       (
           /* IN */ UCHAR  srvs_type,
           /* IN */ UINT8  srvs_indx
       );

#endif /* _H_VCP_INTERNAL */



