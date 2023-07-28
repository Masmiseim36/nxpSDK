/**
 *  \file hap_internal.h
 *
 *  \brief This file handles the context management, handles the callbacks from
 *         the GATT Bearer layer, generates the events to the application
 *         registered.
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_HAP_INTERNAL_
#define _H_HAP_INTERNAL_

/* --------------------------------------------- Header File Inclusion */
#include "GA_hap_api.h"

/* --------------------------------------------- Debug Mapping */
#ifndef HAP_NO_DEBUG
#define HAP_ERR(...)                                GA_debug_error(GA_MODULE_ID_HAP, __VA_ARGS__)
#else  /* HAP_NO_DEBUG */
#define HAP_ERR                                     GA_debug_null
#endif /* HAP_NO_DEBUG */

#ifdef HAP_DEBUG

#define HAP_TRC(...)                                GA_debug_trace(GA_MODULE_ID_HAP, __VA_ARGS__)
#define HAP_INF(...)                                GA_debug_info(GA_MODULE_ID_HAP, __VA_ARGS__)

#define HAP_debug_dump_bytes(data, datalen) GA_debug_dump_bytes(GA_MODULE_ID_HAP, (data), (datalen))

#else /* HAP_DEBUG */

#define HAP_TRC                                     GA_debug_null
#define HAP_INF                                     GA_debug_null

#define HAP_debug_dump_bytes(data, datalen)

#endif /* HAP_DEBUG */

/* --------------------------------------------- Global Definitions */

/* --------------------------------------------- Data types /Structures */
#define HAP_INVALID_VALUE 0xFF

#ifdef HAP_CLIENT

typedef struct _HAP_CE
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
    GA_CHAR_INFO char_info[HAP_CHAR_ID_COUNT];

    /**
     * Current characteristic ID being configured
     */
    UINT8 char_config_id;

    /* HAP generated Context ID for this device */
    HAP_HANDLE has_handle;

    /* Expected Event for the Operation that is active */
    HAP_EVT_ID evt_id;

} HAP_CE;
#endif /* HAP_CLIENT */

#ifdef HAP_SERVER
typedef struct _HAP_SE
{
    /** Flag to indicate if allocated */
    UCHAR state;

    /** Service Id */
    GA_BRR_PRF_HANDLE svc_id_HAS;

    /** Hearing Aid Features */
    GA_BRR_CHR_INST char_id_HA_FEATURES;

    /** Hearing Aid Preset Control Point */
    GA_BRR_CHR_INST char_id_HA_PRESET_CP;

    /** Active Preset Index */
    GA_BRR_CHR_INST char_id_ACTIVE_PRESET_INDEX;

    /** HAP Features */
    HAP_FEATURES features;

}HAP_SE;
#endif /* HAP_SERVER */

/* --------------------------------------------- Macros */
/** HAP Mutex Lock & Unlock */
#define HAP_LOCK()                   GA_MUTEX_LOCK(hap_mutex, HAP)
#define HAP_UNLOCK()                 GA_MUTEX_UNLOCK(hap_mutex, HAP)
#define HAP_LOCK_VOID()              GA_MUTEX_LOCK_VOID(hap_mutex, HAP)
#define HAP_UNLOCK_VOID()            GA_MUTEX_UNLOCK_VOID(hap_mutex, HAP)

/* --------------------------------------------- APIs */
#ifdef HAP_CLIENT
/* Callback registered with GA Bearer for Profile Callbacks only */
GA_RESULT hap_ga_brr_profile_callback
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
 * fetch the HAP Handle.
 */
GA_RESULT hap_reference_context
          (
              /* IN    */ void       * ctx,
              /* OUT   */ HAP_HANDLE * hap_handle_index
          );

/*
 * Fetch the HAP Handle based on the device address.
 */
GA_RESULT hap_search_device_context
          (
              /* IN */   GA_ENDPOINT * device,
              /* OUT */  UINT8       * hap_handle
          );

/* Handle the Service Composition of HAS Service discovered */
void hap_handle_composition
     (
         UINT8                        handle,
         GA_BRR_SERVICE_COMPOSITION * svc_comp
     );

void hap_handle_char_composition
     (
         UINT8 handle,
         GA_BRR_CHARACTERISTIC_COMPOSITION * comp
     );

UINT8 hap_ce_get_char_id_from_uuid(UINT16 uuid);
UINT8 hap_ce_get_char_id_from_handle(UINT16 char_handle, GA_CHAR_INFO * has);
UINT8 hap_ce_get_next_char_config(UINT8 char_id, GA_CHAR_INFO * has);
#endif /* HAP_CLIENT*/

#ifdef HAP_SERVER
/* Callback registered with GA Bearer for Service Callbacks only */
GA_RESULT hap_ga_brr_service_callback
          (
              /* IN */ GA_BRR_DEVICE * device,
              /* IN */ void * context,
              /* IN */ UINT8 event_type,
              /* IN */ UINT16 event_status,
              /* IN */ void * event_data,
              /* IN */ UINT16 event_datalen
          );

/* HAP - Handle Read and Write Request */
void hap_handle_read_write_request
     (
         /* IN */ GA_ENDPOINT        * device,
         /* IN */ GA_BRR_CHR_CONTEXT * chr,
         /* IN */ HAP_EVT_ID           hap_evt_id,
         /* IN */ HAP_EVT            * hap_evt
     );

/* Derive the HAP event ID based on the Request and the Char Instance */
GA_RESULT hap_fetch_evt_id
          (
              /* IN */  UINT8        event_type,
              /* IN */  UINT16       hap_char_inst_id,
              /* OUT */ HAP_EVT_ID * hap_evt_id
          );
#endif /* HAP_SERVER */

#endif /* _H_HAP_INTERNAL_ */

