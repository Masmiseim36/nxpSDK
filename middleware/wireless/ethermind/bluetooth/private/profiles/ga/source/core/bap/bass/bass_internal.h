
/**
 *  \file bass_internal.h
 *
 *  \brief This file handles the context management, handles the callbacks from
 *         the GATT Bearer layer, generates the events to the application
 *         registered.
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BASS_INTERNAL_
#define _H_BASS_INTERNAL_

/* --------------------------------------------- Header File Inclusion */
#include "GA_bass_api.h"

/* --------------------------------------------- Debug Mapping */
#ifndef BASS_NO_DEBUG
#define BASS_ERR(...)          GA_debug_error(GA_MODULE_ID_BASS, __VA_ARGS__)
#else  /* BASS_NO_DEBUG */
#define BASS_ERR               GA_debug_null
#endif /* BASS_NO_DEBUG */

#ifdef BASS_DEBUG

#define BASS_TRC(...)          GA_debug_trace(GA_MODULE_ID_BASS, __VA_ARGS__)
#define BASS_INF(...)          GA_debug_info(GA_MODULE_ID_BASS, __VA_ARGS__)

#define BASS_debug_dump_bytes(data, datalen) GA_debug_dump_bytes(GA_MODULE_ID_BASS, (data), (datalen))

#else  /* BASS_DEBUG */

#define BASS_TRC               GA_debug_null
#define BASS_INF               GA_debug_null

#define BASS_debug_dump_bytes(data, datalen)

#endif /* BASS_DEBUG */

/** By default, unused instance before session is established */
#define BASS_INST_UNUSED                         0xFF

/** Invalid Event, Do not trigger a callback to the app */
#define BASS_EVT_INVALID                         0xFF

/* --------------------------------------------- Global Definitions */

/* --------------------------------------------- Data types /Structures */

#ifdef GA_BASS_CLIENT

/* UUID element for BASS characteristics */
typedef struct _BASS_CHAR_UUID_ELEMENT
{
    /** UUID Value */
    GA_CHAR_UUID uuid;

    /** Count of UUID supportd */
    UINT8 count;

    /** UUID Offset */
    UINT8 offset;

} BASS_CHAR_UUID_ELEMENT;

/**
 * BASS Contexts of remote BASS Server
 */
typedef struct _BASS_CE_CONTEXT
{
    /* Device Endpoint */
    GA_ENDPOINT device;

    /* BASS CE generated Context ID for this device */
    BASS_HANDLE bass_handle;

    /**
     * Characteristic Information
     *
     * TODO:
     * This will be incorrect during Static Allocation. Multiple instances of possible
     * characteristics should be considered.
     */
    GA_DEFINE_GLOBAL_ARRAY(GA_CHAR_INFO, char_info, BASS_CHAR_ID_COUNT);

    /**
     * Total Count of Broadcast Receive State Instances discovered
     */
    UINT8 bc_rx_state_inst_count;

    /**
     * Current characteristic ID being configured/read
     */
    UINT8 char_config_id;

    /**
     * Index of current characteristic ID being configured/read
     */
    UINT8 char_index;

    /* Expected Event for the Operation that is active */
    BASS_CE_EVT_ID evt_id;

    /** Service Range */
    GA_BRR_SVC_RANGE svc_hndl_range;

}BASS_CE_CONTEXT;

#endif /* GA_BASS_CLIENT */

#ifdef GA_BASS_SERVER
typedef struct _BASS_SE_ENTITY
{
    /* Service Instance provided GA Bearer Module during registration */
    GA_BRR_SVC_INST bass_svc_inst_id;

    /* Char Instance provided GA Bearer Module during registration */
    GA_BRR_CHR_INST bass_char_bas_cp_id;
    GA_BRR_CHR_INST bass_char_bc_rx_state_id[BASS_SE_MAX_BC_RX_STATE_ENTITIES];

}BASS_SE_ENTITY;

#endif /* GA_BASS_SERVER */
/* --------------------------------------------- Macros */
#ifdef GA_BASS_CLIENT

/* Lock and Unlock macros for BASS Client */
#define BASS_CE_LOCK()              GA_MUTEX_LOCK(bass_ce_mutex, BASS)
#define BASS_CE_LOCK_VOID()         GA_MUTEX_LOCK_VOID(bass_ce_mutex, BASS)
#define BASS_CE_UNLOCK()            GA_MUTEX_UNLOCK(bass_ce_mutex, BASS)
#define BASS_CE_UNLOCK_VOID()       GA_MUTEX_UNLOCK_VOID(bass_ce_mutex, BASS)

/* Access the Char info in context with the ID and offset */
#define BASS_CHAR_ID(id)             ((id) + bass_uuid[(id)].offset)

#define BASS_CE_SET_BASS_INSTANCE_UNUSED(inst) \
        bass_ce_instance[(inst)].bass_handle = BASS_INST_UNUSED;

#define BASS_CE_SET_EVT(inst, evt) \
        bass_ce_instance[(inst)].evt_id = evt;

#define BASS_CE_IS_BUSY(inst) \
        (BASS_EVT_INVALID != bass_ce_instance[(inst)].evt_id)

#define BASS_CE_SET_EVT_INVALID(inst) \
        bass_ce_instance[(inst)].evt_id = BASS_EVT_INVALID;

/* Set the Handle once complete discovery is done */
#define BASS_CE_SET_BASS_HANDLE(inst) \
        bass_ce_instance[inst].bass_handle = inst;

#define BASS_CE_IS_HANDLE_VALID(bass_handle) \
        (bass_handle < GA_CONFIG_LIMITS(BASS_CE_MAX_INSTANCES))

#define BASS_CE_IS_VALID(inst) \
        ((BASS_CE_IS_HANDLE_VALID(inst)) && (BASS_INST_UNUSED != bass_ce_instance[(inst)].bass_handle))

#define BASS_CE_IS_DEV_SAME(index, device) \
        (0 == GA_mem_cmp((device), &bass_ce_instance[(index)].device, sizeof(GA_ENDPOINT)))

#define BASS_IS_SVC_VALID_BASS(service) \
        (GA_SERVICE_BASS == (service->uuid))

#endif /* GA_BASS_CLIENT */

#ifdef GA_BASS_SERVER
/* Lock and Unlock macros for BASS Server */
#define BASS_SE_LOCK()              GA_MUTEX_LOCK(bass_se_mutex, BASS)
#define BASS_SE_LOCK_VOID()         GA_MUTEX_LOCK_VOID(bass_se_mutex, BASS)
#define BASS_SE_UNLOCK()            GA_MUTEX_UNLOCK(bass_se_mutex, BASS)
#define BASS_SE_UNLOCK_VOID()       GA_MUTEX_UNLOCK_VOID(bass_se_mutex, BASS)

#define BASS_SE_SET_BASS_INSTANCE_UNUSED() \
        bass_se_entity.bass_svc_inst_id = BASS_INST_UNUSED;

#define BASS_SE_SET_BC_RX_STATE_INSTANCE_UNUSED(inst) \
        bass_se_entity.bass_char_bc_rx_state_id[(inst)] = BASS_INST_UNUSED;

#endif /* GA_BASS_SERVER */
/* --------------------------------------------- APIs */

#ifdef GA_BASS_CLIENT
void bass_ce_init_inst(void);

BASS_CE_CONTEXT * bass_ce_alloc_entity
                  (
                      /* IN */   GA_ENDPOINT * device,
                      /* OUT */  BASS_HANDLE * bass_handle_index
                  );
BASS_CE_CONTEXT * bass_ce_search_entity
                  (
                      /* IN */   GA_ENDPOINT * device,
                      /* OUT */  UINT8       * bass_handle_index
                  );
void bass_ce_free_entity(/* IN */  BASS_HANDLE bass_handle_index);

UINT8 bass_ce_get_char_id_from_uuid(UINT16 uuid);
UINT8 bass_ce_get_char_id_from_handle
      (
          UINT16 char_handle,
          BASS_CE_CONTEXT* bass
      );
GA_RESULT bass_ce_get_bc_rx_state_inst_id_from_handle
          (
              UINT16 char_handle,
              BASS_CE_CONTEXT* bass,
              UINT8 * bc_rx_state_inst_id
          );
UINT8 bass_ce_get_next_char_config(UINT8 char_id, BASS_CE_CONTEXT * bass);

GA_RESULT bass_ce_reference_context
          (
              /* IN    */ void  * ctx,
              /* OUT   */ UINT8 * bass_handle
          );

void bass_handle_composition
     (
         UINT8                        handle,
         GA_BRR_SERVICE_COMPOSITION * svc_comp
     );

void bass_handle_char_composition
     (
         UINT8 handle,
         GA_BRR_CHARACTERISTIC_COMPOSITION * comp
     );

/* Callback registered with GA Bearer for Client Callbacks only */
GA_RESULT bass_ce_ga_brr_client_callback
          (
              /* IN */ GA_BRR_DEVICE * device,
              /* IN */ void          * context,
              /* IN */ UINT8           event_type,
              /* IN */ UINT16          event_status,
              /* IN */ void          * event_data,
              /* IN */ UINT16          event_datalen
          );

#endif /* GA_BASS_CLIENT */

#ifdef GA_BASS_SERVER
void bass_se_init_inst(void);

void bass_se_handle_read_write_request
     (
         /* IN */ GA_ENDPOINT        * device,
         /* IN */ GA_BRR_CHR_CONTEXT * chr,
         /* IN */ BASS_SE_EVT_ID       bass_se_evt_id,
         /* IN */ BASS_EVT           * bass_se_evt
     );

GA_RESULT bass_se_fetch_bass_se_handle_evt_id
          (
              /* IN */  UINT8            event_type,
              /* IN */  UINT16           bass_char_inst_id,
              /* OUT */ UINT8          * char_inst,
              /* OUT */ BASS_SE_EVT_ID * bass_se_evt_id
          );

/* Callback registered with GA Bearer for Server Callbacks only */
GA_RESULT bass_se_ga_brr_server_callback
          (
              /* IN */ GA_BRR_DEVICE * device,
              /* IN */ void          * context,
              /* IN */ UINT8           event_type,
              /* IN */ UINT16          event_status,
              /* IN */ void          * event_data,
              /* IN */ UINT16          event_datalen
          );
#endif /* GA_BASS_SERVER */

#endif /* _H_BASS_INTERNAL_ */

