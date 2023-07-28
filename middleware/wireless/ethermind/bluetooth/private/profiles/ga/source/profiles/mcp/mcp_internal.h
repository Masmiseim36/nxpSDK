
/**
 *  \file mcp_internal.h
 *
 *  \brief This file handles the context management, handles the callbacks from
 *         the GATT Bearer layer, generates the events to the application
 *         registered.
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_MCP_INTERNAL_
#define _H_MCP_INTERNAL_

/* --------------------------------------------- Header File Inclusion */
#include "GA_mcp_ce_api.h"
#include "GA_mcp_se_api.h"

#ifdef MCP_SUPPORT_OBJECT_TRANSFER
#include "otc.h"
#include "ots.h"
#endif /* MCP_SUPPORT_OBJECT_TRANSFER */

/* --------------------------------------------- Debug Mapping */
#ifndef MCP_NO_DEBUG
#define MCP_ERR(...)                                GA_debug_error(GA_MODULE_ID_MCP, __VA_ARGS__)
#else  /* MCP_NO_DEBUG */
#define MCP_ERR                                     GA_debug_null
#endif /* MCP_NO_DEBUG */

#ifdef MCP_DEBUG

#define MCP_TRC(...)                                GA_debug_trace(GA_MODULE_ID_MCP, __VA_ARGS__)
#define MCP_INF(...)                                GA_debug_info(GA_MODULE_ID_MCP, __VA_ARGS__)

#define MCP_debug_dump_bytes(data, datalen) GA_debug_dump_bytes(GA_MODULE_ID_MCP, (data), (datalen))

#else /* MCP_DEBUG */

#define MCP_TRC                                     GA_debug_null
#define MCP_INF                                     GA_debug_null

#define MCP_debug_dump_bytes(data, datalen)

#endif /* MCP_DEBUG */

/* --------------------------------------------- Global Definitions */
#if ((defined MCP_CLIENT) || (defined MCP_SERVER))
/** By default, unused instance before session is established */
#define MCP_INST_UNUSED                         0xFFU

/** Invalid Event, Do not trigger a callback to the app */
#define MCP_EVT_INVALID                         0xFFU

#endif /* MCP_CLIENT || MCP_SERVER */

#ifdef MCP_CLIENT
/** Service Type represented as MCP_CE_SVC_TYPE */
#define MCP_CE_SVC_TYPE_GMCS                    0x01U
#define MCP_CE_SVC_TYPE_MCS                     0x02U
#endif /* MCP_CLIENT */
/* --------------------------------------------- Data types /Structures */
#ifdef MCP_CLIENT
typedef struct _MCP_CE_MCS_INFO
{
    /**
     * Service Range
     */
    GA_BRR_SVC_RANGE svc_hndl_range;

    /**
     * Characteristic Information
     */
    GA_CHAR_INFO char_info[MCP_CHAR_ID_COUNT];

    /**
     * Current characteristic ID being configured
     */
    UINT8 char_config_id;

#ifdef MCP_SUPPORT_OBJECT_TRANSFER
    /*
     * This ID is provided by the OTC post discovery of OTS for this svc handle
     * range.
     */
    OTP_CE_SESSION_ID otp_ce_sess_id;
#endif /* MCP_SUPPORT_OBJECT_TRANSFER */

} MCP_CE_MCS_INFO;

typedef struct _MCP_CE_
{
    /* Device Endpoint */
    GA_ENDPOINT device;

    /* Mandatory GMCS instance handles */
    MCP_CE_MCS_INFO gmcs;

    /* MCP CE generated Context ID for this device */
    MCP_HANDLE gmcs_handle;

    /* Expected Event for the Operation that is active */
    MCP_CE_EVT_ID evt_id;

#ifdef MCP_SUPPORT_OBJECT_TRANSFER
    /* MCS instance that has initiated an operation on MCP Control Channel */
    MCP_HANDLE mcs_objproc_handle;
#endif /* MCP_SUPPORT_OBJECT_TRANSFER */
} MCP_CE;

typedef struct _MCP_CE_MCS_
{
    /* MCC generated Context ID for GMCS - Provides reference to GMCS device */
    MCP_HANDLE gmcs_handle;

    /* MCS Session parameters, belonging to device ID above */
    MCP_CE_MCS_INFO mcs;
} MCP_CE_MCS;

typedef UINT8 MCP_CE_SVC_TYPE;
#endif /* MCP_CLIENT */

#ifdef MCP_SERVER
typedef struct _MCP_MCS_ENTITY_
{
    /* Service Instance provided GA Bearer Module during registration */
    GA_BRR_SVC_INST mcs_svc_inst_id;

#ifdef MCP_SUPPORT_OBJECT_TRANSFER
    /* OTS Session ID */
    OTP_SE_SESSION_ID otp_se_sess_id;
#endif /* MCP_SUPPORT_OBJECT_TRANSFER */

    /* Char Instance provided GA Bearer Module during registration */
    GA_BRR_CHR_INST mcs_char_mp_name_id;
    GA_BRR_CHR_INST mcs_char_mp_icon_obj_id;
    GA_BRR_CHR_INST mcs_char_mp_icon_url_id;
    GA_BRR_CHR_INST mcs_char_track_chgd_id;
    GA_BRR_CHR_INST mcs_char_track_title_id;
    GA_BRR_CHR_INST mcs_char_track_dur_id;
    GA_BRR_CHR_INST mcs_char_track_pos_id;
    GA_BRR_CHR_INST mcs_char_pb_speed_id;
    GA_BRR_CHR_INST mcs_char_seek_speed_id;
    GA_BRR_CHR_INST mcs_char_curr_track_seg_obj_id;
    GA_BRR_CHR_INST mcs_char_curr_track_obj_id;
    GA_BRR_CHR_INST mcs_char_next_track_obj_id;
    GA_BRR_CHR_INST mcs_char_parent_group_obj_id;
    GA_BRR_CHR_INST mcs_char_curr_group_obj_id;
    GA_BRR_CHR_INST mcs_char_playing_order_id;
    GA_BRR_CHR_INST mcs_char_playing_order_supp_id;
    GA_BRR_CHR_INST mcs_char_media_state_id;
    GA_BRR_CHR_INST mcs_char_mcp_id;
    GA_BRR_CHR_INST mcs_char_mcp_opc_supp_id;
    GA_BRR_CHR_INST mcs_char_scp_id;
    GA_BRR_CHR_INST mcs_char_search_res_obj_id;
    GA_BRR_CHR_INST mcs_ccid;

#ifdef MCP_SUPPORT_OBJECT_TRANSFER
    /*
     * Indicates that this MCS Session is active, on which OTP transactions
     * are expected.
     */
    UINT8 is_ots_active;
#endif /* MCP_SUPPORT_OBJECT_TRANSFER */

} MCP_MCS_ENTITY;

#endif /* MCP_SERVER */

/* --------------------------------------------- Macros */
#ifdef MCP_CLIENT
/** MCP Client Mutex Lock & Unlock */
#define MCP_CE_LOCK()                   GA_MUTEX_LOCK(mcp_ce_mutex, MCP)
#define MCP_CE_UNLOCK()                 GA_MUTEX_UNLOCK(mcp_ce_mutex, MCP)
#define MCP_CE_LOCK_VOID()              GA_MUTEX_LOCK_VOID(mcp_ce_mutex, MCP)
#define MCP_CE_UNLOCK_VOID()            GA_MUTEX_UNLOCK_VOID(mcp_ce_mutex, MCP)

/* Validate the Service UUID - GMCS/MCS/OTS */
#define MCP_IS_SVC_GMCS(service) \
        (GA_SERVICE_GMCS == (service->uuid))

#define MCP_IS_SVC_MCS(service) \
        (GA_SERVICE_MCS == (service->uuid))

#define MCP_IS_SVC_OTS(service) \
        (GA_SERVICE_UUID_OTS == (service->uuid))

#define MCP_IS_SVC_GMCS_OR_MCS(service) \
        ((GA_TRUE == MCP_IS_SVC_GMCS(service)) || (GA_TRUE == MCP_IS_SVC_MCS(service)))

#define MCP_CE_SET_MCS_INSTANCE_UNUSED(inst) \
        mcp_ce_mcs_instance[(inst)].gmcs_handle = MCP_INST_UNUSED;

#define MCP_CE_SET_GMCS_INSTANCE_UNUSED(inst) \
        mcp_ce_instance[(inst)].gmcs_handle = MCP_INST_UNUSED;

#define MCP_CE_SET_EVT_INVALID(inst) \
        mcp_ce_instance[(inst)].evt_id = MCP_EVT_INVALID;

#define MCP_CE_SET_GMCS_HANDLE(gmcs_handle_index, gmcs_handle) \
        ((gmcs_handle) = ((gmcs_handle_index) | MCP_CE_GMCS_ENABLE))

#define MCP_CE_GET_GMCS_HANDLE_INDEX_FROM_HANDLE(gmcs_handle, gmcs_handle_index) \
        ((gmcs_handle_index) = ((gmcs_handle) & 0x7FU))

#define MCP_CE_STRIP_GMCS_MASK(mcp_handle) \
        ((mcp_handle) &= 0x7FU);

#define MCP_CE_SET_EVT(inst, evt) \
        mcp_ce_instance[(inst)].evt_id = (evt);

/* Validate if Session Setup is ongoing or only discovery */
#define MCP_CE_IS_GMCS_SESSION_SETUP_ONGOING(evt) \
        (MCC_GMCS_SESSION_SETUP_EVT == (evt))

#define IS_SVC_RANGE_INVALID(svc_handle_range) \
        (((svc_handle_range.start) == 0x0000U) && ((svc_handle_range.end) == 0x0000U))

#define MCP_CE_IS_GMCS_DEV_SAME(index, device) \
        (0 == EM_mem_cmp((device), &mcp_ce_instance[(index)].device, sizeof(GA_ENDPOINT)))

#define MCP_CE_IS_GMCS_HANDLE_VALID(mcp_handle) \
        ((mcp_handle) < GA_CONFIG_LIMITS(MCP_MAX_CONTEXTS))

#define MCP_CE_IS_MCS_HANDLE_VALID(mcp_handle) \
        ((mcp_handle) < GA_CONFIG_LIMITS(MCP_MAX_MCS_CONTEXTS))

#define MCP_CE_IS_GMCS_VALID(inst) \
        ((MCP_CE_IS_GMCS_HANDLE_VALID(inst)) && (MCP_INST_UNUSED != mcp_ce_instance[(inst)].gmcs_handle))

/* If associated with GMCS, then it is a valid in-use MCS Session */
#define MCP_CE_IS_MCS_VALID(inst) \
        ((MCP_CE_IS_MCS_HANDLE_VALID(inst)) && (MCP_INST_UNUSED != mcp_ce_mcs_instance[(inst)].gmcs_handle))

#define MCP_CE_IS_BUSY(inst) \
        (MCP_EVT_INVALID != mcp_ce_instance[(inst)].evt_id)

#define MCP_CE_MCS_IS_ACTIVE_ON_OTP(inst) \
        (MCP_INST_UNUSED != mcp_ce_instance[(inst)].mcs_objproc_handle)

#define MCP_CE_IS_SVC_HANDLE_RANGE_INVALID(srv_info) \
        (((srv_info).range.start == 0x0000U) && ((srv_info).range.end == 0x0000U))

/* Set the discovered Handle range */
#define MCP_CE_SET_GMCS_SVC_HANDLE_RANGE(inst, svc_handle_range) \
        mcp_ce_instance[(inst)].gmcs.svc_hndl_range.start = (svc_handle_range.start); \
        mcp_ce_instance[(inst)].gmcs.svc_hndl_range.end = (svc_handle_range.end);

/* Set the Session ID once complete discovery is done */
#define MCP_CE_SET_GMCS_SESS_ID(inst) \
        mcp_ce_instance[inst].gmcs_handle = (inst);

#define MCP_CE_IS_GMCS_MEDIA_PLAYER_NAME_HANDLE(instance, char_hndl) \
        ((char_hndl) == mcp_ce_instance[(instance)].gmcs.media_player_name_hndl)

#define MCP_CE_IS_GMCS_TRACK_CHGD_HANDLE(instance, char_hndl) \
        ((char_hndl) == mcp_ce_instance[(instance)].gmcs.track_changed_hndl)

#define MCP_CE_IS_GMCS_TRACK_TITLE_HANDLE(instance, char_hndl) \
        ((char_hndl) == mcp_ce_instance[(instance)].gmcs.track_title_hndl)

#define MCP_CE_IS_GMCS_TRACK_DURATION_HANDLE(instance, char_hndl) \
        ((char_hndl) == mcp_ce_instance[(instance)].gmcs.track_duration_hndl)

#define MCP_CE_IS_GMCS_TRACK_POS_HANDLE(instance, char_hndl) \
        ((char_hndl) == mcp_ce_instance[(instance)].gmcs.track_pos_hndl)

#define MCP_CE_IS_GMCS_PLAYBACK_SPEED_HANDLE(instance, char_hndl) \
        ((char_hndl) == mcp_ce_instance[(instance)].gmcs.playback_speed_hndl)

#define MCP_CE_IS_GMCS_SEEKING_SPEED_HANDLE(instance, char_hndl) \
        ((char_hndl) == mcp_ce_instance[(instance)].gmcs.seeking_speed_hndl)

#define MCP_CE_IS_GMCS_CURR_TRACK_OBJ_ID_HANDLE(instance, char_hndl) \
        ((char_hndl) == mcp_ce_instance[(instance)].gmcs.curr_track_obj_ID_hndl)

#define MCP_CE_IS_GMCS_NEXT_TRACK_OBJ_ID_HANDLE(instance, char_hndl) \
        ((char_hndl) == mcp_ce_instance[(instance)].gmcs.next_track_obj_ID_hndl)

#define MCP_CE_IS_GMCS_PARENT_GRP_OBJ_ID_HANDLE(instance, char_hndl) \
        ((char_hndl) == mcp_ce_instance[(instance)].gmcs.parent_group_obj_ID_hndl)

#define MCP_CE_IS_GMCS_CURR_GRP_OBJ_ID_HANDLE(instance, char_hndl) \
        ((char_hndl) == mcp_ce_instance[(instance)].gmcs.current_group_obj_ID_hndl)

#define MCP_CE_IS_GMCS_PLAYING_ORDER_HANDLE(instance, char_hndl) \
        ((char_hndl) == mcp_ce_instance[(instance)].gmcs.playing_order_hndl)

#define MCP_CE_IS_GMCS_MEDIA_STATE_HANDLE(instance, char_hndl) \
        ((char_hndl) == mcp_ce_instance[(instance)].gmcs.media_state_hndl)

#define MCP_CE_IS_GMCS_MCP_HANDLE(instance, char_hndl) \
        ((char_hndl) == mcp_ce_instance[(instance)].gmcs.media_control_point_hndl)

#define MCP_CE_IS_GMCS_MCP_OPC_SUPP_HANDLE(instance, char_hndl) \
        ((char_hndl) == mcp_ce_instance[(instance)].gmcs.mcp_opc_supp_hndl)

#define MCP_CE_IS_GMCS_SCP_HANDLE(instance, char_hndl) \
        ((char_hndl) == mcp_ce_instance[(instance)].gmcs.scp_hndl)

#define MCP_CE_IS_GMCS_SEARCH_RES_OBJ_ID_HANDLE(instance, char_hndl) \
        ((char_hndl) == mcp_ce_instance[(instance)].gmcs.search_res_obj_ID_hndl)

#define MCP_CE_IS_MCS_MEDIA_PLAYER_NAME_HANDLE(instance, char_hndl) \
        ((char_hndl) == mcp_ce_mcs_instance[(instance)].mcs.media_player_name_hndl)

#define MCP_CE_IS_MCS_TRACK_CHGD_HANDLE(instance, char_hndl) \
        ((char_hndl) == mcp_ce_mcs_instance[(instance)].mcs.track_changed_hndl)

#define MCP_CE_IS_MCS_TRACK_TITLE_HANDLE(instance, char_hndl) \
        ((char_hndl) == mcp_ce_mcs_instance[(instance)].mcs.track_title_hndl)

#define MCP_CE_IS_MCS_TRACK_DURATION_HANDLE(instance, char_hndl) \
        ((char_hndl) == mcp_ce_mcs_instance[(instance)].mcs.track_duration_hndl)

#define MCP_CE_IS_MCS_TRACK_POS_HANDLE(instance, char_hndl) \
        ((char_hndl) == mcp_ce_mcs_instance[(instance)].mcs.track_pos_hndl)

#define MCP_CE_IS_MCS_PLAYBACK_SPEED_HANDLE(instance, char_hndl) \
        ((char_hndl) == mcp_ce_mcs_instance[(instance)].mcs.playback_speed_hndl)

#define MCP_CE_IS_MCS_SEEKING_SPEED_HANDLE(instance, char_hndl) \
        ((char_hndl) == mcp_ce_mcs_instance[(instance)].mcs.seeking_speed_hndl)

#define MCP_CE_IS_MCS_CURR_TRACK_OBJ_ID_HANDLE(instance, char_hndl) \
        ((char_hndl) == mcp_ce_mcs_instance[(instance)].mcs.curr_track_obj_ID_hndl)

#define MCP_CE_IS_MCS_NEXT_TRACK_OBJ_ID_HANDLE(instance, char_hndl) \
        ((char_hndl) == mcp_ce_mcs_instance[(instance)].mcs.next_track_obj_ID_hndl)

#define MCP_CE_IS_MCS_PARENT_GRP_OBJ_ID_HANDLE(instance, char_hndl) \
        ((char_hndl) == mcp_ce_mcs_instance[(instance)].mcs.parent_group_obj_ID_hndl)

#define MCP_CE_IS_MCS_CURR_GRP_OBJ_ID_HANDLE(instance, char_hndl) \
        ((char_hndl) == mcp_ce_mcs_instance[(instance)].mcs.current_group_obj_ID_hndl)

#define MCP_CE_IS_MCS_PLAYING_ORDER_HANDLE(instance, char_hndl) \
        ((char_hndl) == mcp_ce_mcs_instance[(instance)].mcs.playing_order_hndl)

#define MCP_CE_IS_MCS_MEDIA_STATE_HANDLE(instance, char_hndl) \
        ((char_hndl) == mcp_ce_mcs_instance[(instance)].mcs.media_state_hndl)

#define MCP_CE_IS_MCS_MCP_HANDLE(instance, char_hndl) \
        ((char_hndl) == mcp_ce_mcs_instance[(instance)].mcs.media_control_point_hndl)

#define MCP_CE_IS_MCS_MCP_OPC_SUPP_HANDLE(instance, char_hndl) \
        ((char_hndl) == mcp_ce_mcs_instance[(instance)].mcs.mcp_opc_supp_hndl)

#define MCP_CE_IS_MCS_SCP_HANDLE(instance, char_hndl) \
        ((char_hndl) == mcp_ce_mcs_instance[(instance)].mcs.scp_hndl)

#define MCP_CE_IS_MCS_SEARCH_RES_OBJ_ID_HANDLE(instance, char_hndl) \
        ((char_hndl) == mcp_ce_mcs_instance[(instance)].mcs.search_res_obj_ID_hndl)

#ifdef MCP_SUPPORT_OBJECT_TRANSFER
/*
 * It is not a valid Object ID, As per MCP spec, can happen for below reasons:
 * 1. No media Icon/current track/next track or
 * 2. Not created parent/current group or
 * 3. No search results found.
 */
#define MCP_CE_IS_OBJ_ID_VALID(event_data, event_datalen) \
        ((OBJ_ID_LEN == (event_datalen)) && (NULL != (event_data)))

#endif /* MCP_SUPPORT_OBJECT_TRANSFER */
#endif /* MCP_CLIENT */

#ifdef MCP_SERVER
 /** MCP Server Mutex Lock & Unlock */
#define MCP_SE_LOCK()                   GA_MUTEX_LOCK(mcp_se_mutex, MCP)
#define MCP_SE_UNLOCK()                 GA_MUTEX_UNLOCK(mcp_se_mutex, MCP)
#define MCP_SE_LOCK_VOID()              GA_MUTEX_LOCK_VOID(mcp_se_mutex, MCP)
#define MCP_SE_UNLOCK_VOID()            GA_MUTEX_UNLOCK_VOID(mcp_se_mutex, MCP)

#ifdef MCP_SUPPORT_OBJECT_TRANSFER
/*
 * When an Object ID read is received and a valid Obj ID is sent, set this as
 * active.
 */
#define MCP_SE_SET_MCS_ACTIVE_ON_OTP(instance, isActive) \
        mcs_entity[(instance)].is_ots_active = (isActive);

#define MCP_SE_SET_ALL_MCS_ACTIVE_ON_OTP(isActive) \
        { \
            UINT8 i = 0U; \
            for (i = 0U; i < MCP_SE_MAX_MCS_SER_ENTITIES; i++) \
            { \
                mcs_entity[i].is_ots_active = (isActive); \
            } \
        }

/* Validates if MCS session is expecting any event on OTP */
#define MCP_SE_IS_MCS_ACTIVE_ON_OTS(instance) \
        (GA_TRUE == mcs_entity[(instance)].is_ots_active)

#endif /* MCP_SUPPORT_OBJECT_TRANSFER */
#endif /* MCP_SERVER */
/* --------------------------------------------- APIs */
#ifdef MCP_CLIENT
#ifdef MCP_SUPPORT_OBJECT_TRANSFER
/* Discover OTS included as Secondary in the range */
GA_RESULT mcp_ce_disc_ss_ots_svc
          (
              MCP_HANDLE         mcp_handle,
              GA_ENDPOINT      * dev,
              GA_BRR_SVC_RANGE * ots_handle_range
          );
#endif /* MCP_SUPPORT_OBJECT_TRANSFER */
/* Callback registered with GA Bearer for Client Callbacks only */
GA_RESULT mcp_ce_ga_brr_client_callback
          (
              /* IN */ GA_BRR_DEVICE * device,
              /* IN */ void          * context,
              /* IN */ UINT8           event_type,
              /* IN */ UINT16          event_status,
              /* IN */ void          * event_data,
              /* IN */ UINT16          event_datalen
          );

#ifdef MCP_SUPPORT_OBJECT_TRANSFER
/* Callback registered with OTP CE for Client Callbacks only */
GA_RESULT mcp_ce_ga_otp_ce_callback
          (
              OTP_CE_SESSION_ID   otp_ce_sess_id,
              OTP_CE_EVT_ID       evt_id,
              GA_RESULT           evt_status,
              void              * evt_data,
              UINT16              evt_datalen
          );
#endif /* MCP_SUPPORT_OBJECT_TRANSFER */

/* Utility Functions wrt MCP CE context saving */

/* Initializes GMCS and MCS instances to default values */
void mcp_ce_init_inst (void);

/* Set the expected Event ID to Invalid indicating no event is expected */
void mcp_ce_reset_inst_evt_id
     (
         /* IN */  MCP_HANDLE mcp_handle
     );

/* Return the context if session setup already for this device, else NULL */
MCP_CE * mcp_ce_search_entity
         (
             /* IN */   GA_ENDPOINT * device,
             /* OUT */  UINT8       * mcp_handle_index
         );

/* Allocate a new context for this device */
MCP_CE * mcp_ce_alloc_entity
         (
             /* IN */   GA_ENDPOINT * device,
             /* OUT */  MCP_HANDLE  * handle
         );

/* Free up the device entity */
void mcp_ce_free_entity(/* IN */  MCP_HANDLE mcp_handle);

/* Allocate a new context for this MCS entity */
MCP_CE_MCS * mcp_ce_alloc_mcs_entity
             (
                 /* OUT */  MCP_HANDLE * handle
             );

/* Search all the MCS instances associated with this device address */
GA_RESULT mcp_ce_search_mcs_entity
          (
              /* IN */   GA_ENDPOINT * device,
              /* OUT */  UINT8       * num_of_mcs,
              /* OUT */  MCP_HANDLE  * mcp_handle_list
          );

/* Free up the MCS entity */
void mcp_ce_free_mcs_entity(/* IN */  MCP_HANDLE mcp_handle);

/* Return the context if session setup already for this device, else NULL */
MCP_CE_MCS * mcp_ce_search_mcs_entity_with_same_handle_range
             (
                 /* IN */   GA_ENDPOINT      * device,
                 /* IN */   GA_BRR_SVC_RANGE * mcs_handle_range,
                 /* OUT */  UINT8            * mcp_handle_index
             );

/*
 * Based on the context saved before calling function to GA BRR layer,
 * fetch the MCP Handle and correspondingly the GMCS/MCS service type.
 */
GA_RESULT mcp_ce_reference_context
          (
              /* IN    */ void            * ctx,
              /* OUT   */ UINT8           * mcp_handle_index,
              /* OUT   */ MCP_CE_SVC_TYPE * mcp_svc_type
          );

/* Validates the input MCP Handle and derives the corresponding context */
GA_RESULT mcp_ce_validate_and_fetch_context
          (
              /* IN */   MCP_HANDLE    mcp_handle,
              /* OUT */  UINT8      *  gmcs_handle_index,
              /* OUT */  void       ** ctx
          );

/* Service, Char, Desc Results Update for a MCS Session */
void mcp_ce_handle_composition
     (
         /* IN */ MCP_CE_MCS_INFO * mcs,
         /* IN */  GA_BRR_CHR_INFO * chr_info,
         /* IN */  UINT8             chr_cnt
     );

void mcp_ce_handle_char_composition
     (
         /* IN */ MCP_CE_MCS_INFO * mcs,
         /* IN */ GA_BRR_CHARACTERISTIC_COMPOSITION * comp
     );

/*
 * This function gets the Char Handle based on the Char UUID for GMCS/MCS
 * Session.
 */
GA_RESULT mcp_ce_action_on_char_handle_for_inst
          (
              UINT8             set_handle,
              MCP_CE_MCS_INFO * mcs,
              UINT16            char_uuid,
              UINT16          * char_handle
          );

/*
 * For a event receieved from peer without having initiated from client, like
 * Notifications/Indications, this function iterates through GMCS and MCS
 * based on the char handle to find the handle and the corresponding event.
 */
GA_RESULT mcp_ce_reference_char_info
          (
              /* IN */   GA_BRR_CHR_HANDLE   char_hndl,
              /* IN */   UINT8               mcp_handle_index,
              /* OUT */  MCP_HANDLE        * mcp_handle,
              /* OUT */  MCP_CE_EVT_ID     * mcp_ce_evt_id
          );

/* Configure Context performed in a chain */
GA_RESULT mcp_ce_configure_context
          (
              /* IN */ MCP_HANDLE   mcp_handle,
              /* IN */ UINT8        mcs_ntf_flag,
              /* IN */ void       * ctx
          );

/* Get Char Ref ID for this UUID */
UINT8 mcp_ce_get_char_id_from_uuid
      (
          /* IN */ UINT16 uuid
      );

/* Get Char Ref ID for this GMCS/MCS instance based on the Char Handle */
UINT8 mcp_ce_get_char_id_from_handle
      (
          /* IN */ UINT16 char_handle,
          /* IN */ MCP_CE_MCS_INFO * mcs
      );

/* Get Next Char Ref ID for this GMCS/MCS instance based on the Char Ref ID */
UINT8 mcp_ce_get_next_char_config
      (
          /* IN */ UINT8 char_id,
          /* IN */ MCP_CE_MCS_INFO * mcs
      );

/* Begin Notification Disable for this GMCS/MCS instance */
GA_RESULT mcp_ce_begin_disable_ntf
          (
              /* IN */  MCP_HANDLE handle,
              /* IN */  GA_BRR_DEVICE * device,
              /* IN */  MCP_CE_MCS_INFO * mcs
          );

#ifdef MCP_SUPPORT_OBJECT_TRANSFER
/* Triggers OTP CE function to read the Object attribute requested over OTP */
GA_RESULT mcp_ce_read_ots_obj_attr
          (
              /* IN */  OTP_CE_SESSION_ID  otc_sess_id,
              /* IN */  MCP_CE_OBJ_ATTR_ID attr_id
          );

/* Perform OACP read over OTP */
GA_RESULT mcp_ce_otp_ce_oacp_read_proc
          (
              OTP_CE_SESSION_ID otc_sess_id,
              UINT32            offset,
              UINT32            len,
              MCP_CE_OBJ_TYPE   mcc_obj_type
          );

/* Perform OLF - No Filter on all Instances over OTP */
GA_RESULT mcp_ce_otp_ce_olf_no_filter_proc
          (
              OTP_CE_SESSION_ID otp_ce_sess_id
          );

/* Perform OLCP - Goto over OTP */
GA_RESULT mcp_ce_otp_ce_olcp_goto_proc
          (
              OTP_CE_SESSION_ID   otp_ce_sess_id,
              UINT8             * obj_id
          );

/*
 * Based on the input OTP CE Session ID, the active MCP Handle waiting for
 * transaction on OTP is returned if any.
 */
GA_RESULT mcp_ce_get_mcp_context_active_on_otp
          (
              /* IN */  OTP_CE_SESSION_ID   otp_ce_sess_id,
              /* IN */  UINT8               evt,
              /* OUT */ MCP_HANDLE        * mcp_handle,
              /* OUT */ GA_ENDPOINT       *  dev
          );

/* Based on the expected event ID, get the Attribute ID */
GA_RESULT mcp_ce_get_obj_attr_id_from_evt
          (
              /* IN */   MCP_CE_EVT_ID        mcp_ce_evt_id,
              /* OUT */  MCP_CE_OBJ_ATTR_ID * mcp_ce_obj_attr_id
          );

/* Based on the Object Type receved, Get the Event ID */
GA_RESULT mcp_ce_get_evt_id_from_obj_type
          (
              /* IN */   MCP_CE_OBJ_TYPE   mcc_obj_type,
              /* OUT */  MCP_CE_EVT_ID   * mcp_ce_evt_id
          );

#endif /* MCP_SUPPORT_OBJECT_TRANSFER */
#endif /* MCP_CLIENT */

#ifdef MCP_SERVER
/* Callback registeted with the GA Bearer Module - Server */
GA_RESULT mcp_se_ga_brr_server_callback
          (
              /* IN */ GA_BRR_DEVICE * device,
              /* IN */ void          * context,
              /* IN */ UINT8           event_type,
              /* IN */ UINT16          event_status,
              /* IN */ void          * event_data,
              /* IN */ UINT16          event_datalen
          );

#ifdef MCP_SUPPORT_OBJECT_TRANSFER
/* Callback registered with the OTS Module */
GA_RESULT mcp_se_otp_se_callback
          (
              /* IN */ OTP_SE_SESSION_ID   otp_se_sess_id,
              /* IN */ GA_ENDPOINT       * device,
              /* IN */ void              * ctx,
              /* IN */ OTP_SE_EVT_ID       evt_id,
              /* IN */ OTP_RESULT          evt_status,
              /* IN */ void              * evt_data,
              /* IN */ UINT16              evt_datalen
          );
#endif /* MCP_SUPPORT_OBJECT_TRANSFER */

/*
 * When read/write request is received, trigger a callback and obtain the read
 * response data from app, pack it and send as response to peer.
 * For a write request, send the response.
 */
void mcp_se_handle_read_write_request
     (
         /* IN */ GA_ENDPOINT        * device,
         /* IN */ GA_BRR_CHR_CONTEXT * chr,
         /* IN */ MCP_SE_EVT_ID        mcp_se_evt_id,
         /* IN */ MCP_SE_EVT         * mcp_se_evt
     );

/*
 * Fetch a free MCS session instance position in MCS_SVC_REG DB to add the
 * service instance.
 */
GA_RESULT mcp_se_fetch_free_mcs_ctx
          (
              /* OUT */ MCP_SE_HANDLE * mcp_se_handle
          );

/*
 * Fetch MCS Char Instance ID provided during registration of service by the GA Brr.
 * Based on the char UUID, char instance ID is retrieved and used.
 */
GA_RESULT mcp_se_fetch_char_inst_id_from_char_uuid
          (
              /* IN */  MCP_SE_HANDLE     mcp_se_handle,
              /* IN */  UINT16            char_uuid,
              /* OUT */ GA_BRR_CHR_INST * mcs_char_inst_id
          );

/*
 * This function retrieves the MCS session ID and Event ID based on the char
 * instance ID and the event type. This is called when read/write request is
 * triggered from peer MCP - Client.
 */
GA_RESULT mcp_se_fetch_mcp_se_handle_evt_id
          (
              /* IN */  UINT8           event_type,
              /* IN */  UINT16          mcs_char_inst_id,
              /* OUT */ MCP_SE_HANDLE * mcp_se_handle,
              /* OUT */ MCP_SE_EVT_ID * mcp_se_evt_id
          );

/* Resets complete DB - During init and shutdown */
void mcp_se_init_inst (void);

#ifdef MCP_SUPPORT_OBJECT_TRANSFER
void mcp_se_handle_otp_se_read_write_request
     (
         /* IN */ OTP_SE_SESSION_ID    otp_se_sess_id,
         /* IN */ GA_ENDPOINT        * device,
         /* IN */ GA_BRR_CHR_CONTEXT * chr,
         /* IN */ OTP_SE_EVT_ID        otp_se_evt_id,
         /* IN */ MCP_SE_EVT         * mcs_evt
     );

void mcp_se_handle_otp_se_data_channel_evt
     (
         /* IN */ OTP_SE_SESSION_ID   otp_se_sess_id,
         /* IN */ GA_ENDPOINT       * device,
         /* IN */ OTP_SE_EVT_ID       otp_se_evt_id,
         /* IN */ GA_RESULT           evt_status,
         /* IN */ MCP_SE_EVT        * mcp_se_evt
     );

GA_RESULT mcp_se_get_evt_for_otp_se_evt
          (
              /* IN */  OTP_SE_EVT_ID   otp_se_evt_id,
              /* OUT */ MCP_SE_EVT_ID * mcs_evt_id
          );

GA_RESULT mcp_se_fetch_mcp_se_handle_from_otp_se_id
          (
              OTP_SE_SESSION_ID   otp_se_sess_id,
              MCP_SE_HANDLE     * mcp_se_handle,
              UINT8               is_ots_active_chk
          );
#endif /* MCP_SUPPORT_OBJECT_TRANSFER */
#endif /* MCP_SERVER */

#endif /* _H_MCP_INTERNAL_ */

