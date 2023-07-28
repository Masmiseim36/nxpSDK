/**
 *  \file tmap_internal.h
 *
 *  \brief This file handles the context management, handles the callbacks from
 *         the GATT Bearer layer, generates the events to the application
 *         registered.
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_TMAP_INTERNAL_
#define _H_TMAP_INTERNAL_

/* --------------------------------------------- Header File Inclusion */
#include "GA_tmap_api.h"

/* --------------------------------------------- Debug Mapping */
#ifndef TMAP_NO_DEBUG
#define TMAP_ERR(...)                                GA_debug_error(GA_MODULE_ID_TMAP, __VA_ARGS__)
#else  /* TMAP_NO_DEBUG */
#define TMAP_ERR                                     GA_debug_null
#endif /* TMAP_NO_DEBUG */

#ifdef TMAP_DEBUG

#define TMAP_TRC(...)                                GA_debug_trace(GA_MODULE_ID_TMAP, __VA_ARGS__)
#define TMAP_INF(...)                                GA_debug_info(GA_MODULE_ID_TMAP, __VA_ARGS__)

#define TMAP_debug_dump_bytes(data, datalen) GA_debug_dump_bytes(GA_MODULE_ID_TMAP, (data), (datalen))

#else /* TMAP_DEBUG */

#define TMAP_TRC                                     GA_debug_null
#define TMAP_INF                                     GA_debug_null

#define TMAP_debug_dump_bytes(data, datalen)

#endif /* TMAP_DEBUG */

/* --------------------------------------------- Global Definitions */

/* --------------------------------------------- Data types /Structures */
#define TMAP_INVALID_VALUE 0xFF

#ifdef TMAP_CLIENT

typedef struct _TMAP_CE
{
    /* Device Endpoint */
    GA_ENDPOINT device;

    /**
     * Service Range
     */
    GA_BRR_SVC_RANGE svc_hndl_range;

    /**
     * Characteristic Information
     */
    GA_CHAR_INFO char_info[TMAP_CHAR_ID_COUNT];

    /* HAP generated Context ID for this device */
    TMAP_HANDLE tmap_handle;

    /* Expected Event for the Operation that is active */
    TMAP_EVT_ID evt_id;

} TMAP_CE;

#endif /* TMAP_CLIENT */

#ifdef TMAP_SERVER
typedef struct _TMAP_SE
{
    /** Flag to indicate if allocated */
    UCHAR state;

    /** TMAS Service ID */
    GA_BRR_PRF_HANDLE svc_id_TMAS;

    /** TMAP Role Char Instance ID */
    GA_BRR_CHR_INST char_id_TMAP_ROLE;

    /** TMAP Role */
    TMAP_ROLE role;

}TMAP_SE;
#endif /* TMAP_SERVER */

/* --------------------------------------------- Macros */
/** TMAP Mutex Lock & Unlock */
#define TMAP_LOCK()                   GA_MUTEX_LOCK(tmap_mutex, TMAP)
#define TMAP_UNLOCK()                 GA_MUTEX_UNLOCK(tmap_mutex, TMAP)
#define TMAP_LOCK_VOID()              GA_MUTEX_LOCK_VOID(tmap_mutex, TMAP)
#define TMAP_UNLOCK_VOID()            GA_MUTEX_UNLOCK_VOID(tmap_mutex, TMAP)

/* --------------------------------------------- APIs */
#ifdef TMAP_CLIENT
/* Callback registered with GA Bearer for Profile Callbacks only */
GA_RESULT tmap_ga_brr_profile_callback
          (
              /* IN */ GA_BRR_DEVICE * device,
              /* IN */ void          * context,
              /* IN */ UINT8           event_type,
              /* IN */ UINT16          event_status,
              /* IN */ void          * event_data,
              /* IN */ UINT16          event_datalen
          );

/*
 * Based on the context saved before calling function to GA BRR layer,
 * fetch the TMAP Handle.
 */
GA_RESULT tmap_reference_context
          (
              /* IN    */ void        * ctx,
              /* OUT   */ TMAP_HANDLE * tmap_handle_index
          );

/*
 * Fetch the TMAP Handle based on the device address.
 */
GA_RESULT tmap_search_device_context
          (
              /* IN */   GA_ENDPOINT * device,
              /* OUT */  UINT8       * tmap_handle
          );

#endif /* TMAP_CLIENT*/

#ifdef TMAP_SERVER
/* Callback registered with GA Bearer for Service Callbacks only */
GA_RESULT tmap_ga_brr_service_callback
          (
              /* IN */ GA_BRR_DEVICE * device,
              /* IN */ void * context,
              /* IN */ UINT8 event_type,
              /* IN */ UINT16 event_status,
              /* IN */ void * event_data,
              /* IN */ UINT16 event_datalen
          );

/* Derive the TMAP event ID based on the Request and the Char Instance */
GA_RESULT tmap_fetch_evt_id
          (
              /* IN */  UINT8         event_type,
              /* IN */  UINT16        tmap_char_inst_id,
              /* OUT */ TMAP_EVT_ID * tmap_evt_id
          );
#endif /* TMAP_SERVER */

#endif /* _H_TMAP_INTERNAL_ */

