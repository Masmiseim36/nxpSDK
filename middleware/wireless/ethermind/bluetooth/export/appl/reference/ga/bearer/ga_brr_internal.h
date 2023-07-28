
/**
 *  \file ga_brr_internal.h
 *
 *  \brief This file defines the Generic Audio Bearer Internal Interface - includes
 *  Data Structures and Methods.
 */

/**
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_GA_BRR_INTERNAL_
#define _H_GA_BRR_INTERNAL_

/* --------------------------------------------- Header File Inclusion */
#include "GA_bearer_api.h"
#include "ga_brr_utils.h"
#include "ga_brr_pl.h"

/** TODO :
 * Move this section to some other location
 */
#if 0
#define GA_BRR_ERR(...)  printf(__VA_ARGS__)
#define GA_BRR_TRC(...)  printf(__VA_ARGS__)
#define GA_BRR_INF(...)  printf(__VA_ARGS__)
#else /* 0 */
#define GA_BRR_ERR(...)               EM_debug_error(GA_MODULE_ID,__VA_ARGS__)
#define GA_BRR_TRC(...)               EM_debug_trace(GA_MODULE_ID,__VA_ARGS__)
#define GA_BRR_INF(...)               EM_debug_info(GA_MODULE_ID,__VA_ARGS__)
#define GA_brr_debug_dump_bytes(b,l)  EM_debug_dump_bytes(0, (b), (l))
#endif /* 0 */


/* --------------------------------------------- Global Definitions */
/* ============================= Limits ============================== */
#define GA_BRR_MAX_PROFILE_CNTX                                   10
#define GA_BRR_MAX_SERVICE_CNTX                                   20
#define GA_BRR_MAX_INCLUDE_SERVICE_CNTX                           5

/* Below count is set to 11, DB has 9 Primary and 2 Secondary Service */
#define GA_BRR_MAX_SERVICE_UUID_CNTX                              150
#define GA_BRR_MAX_CHAR_UUID_CNTX                                 250

/* --------------------------------------------- Data types /Structures */

typedef struct _GA_BRR_PROFILE_CNTX_
{
    /**
     * Profile Type
     */
    UINT32  pid;

    /**
     * Profile Callback
     */
    GA_BRR_PROFILE_CB  pid_cb;
}GA_BRR_PROFILE_CNTX;


typedef struct _GA_BRR_SERVICE_CNTX_
{
    /**
     * Service ID
     */
    GA_BRR_SVC_INST   sid;

    /**
     * First Instance of a Char ID for this Service
     */
    GA_BRR_CHR_INST   s_cid;

    /**
     * End Instance of a Char ID for this Service
     */
    GA_BRR_CHR_INST   e_cid;

    /**
     * Service UUID
     */
    UINT16   s_uuid;

    /**
     * Service Type:
     * - Primary
     * - Secondary
     */
    UCHAR   s_type;

    /**
     * Service Callback
     */
    GA_BRR_SERVICE_CB  sid_cb;
}GA_BRR_SERVICE_CNTX;

typedef struct _GA_BRR_SERVICE_UUID_CNTX_
{
    /**
     * Service UUID
     */
    GA_BRR_UUID   s_uuid;

    /**
     * Last Known Service ID Occurence of this UUID
     * LastKnownId
     */
    GA_BRR_SVC_INST lkid;
}GA_SERVICE_UUID_CNTX;

typedef struct _GA_CHAR_UUID_CNTX_
{
    /**
     * Characterisitc UUID
     */
    GA_BRR_UUID   c_uuid;

    /**
     * Last Known Characteristic ID Occurence of this UUID
     * LastKnownId
     */
    GA_BRR_CHR_INST lkid;
}GA_CHAR_UUID_CNTX;


typedef struct _GA_BRR_INCLUDE_SERVICE_CNTX_
{
    /**
     * Inlcude Service ID
     */
    GA_BRR_SVC_INST   isid;

    /**
     * Parent Primary Service ID
     */
    GA_BRR_SVC_INST   sid;

    /**
     * Included Service UUID
     */
    UINT16   is_uuid;

}GA_BRR_INCLUDE_SERVICE_CNTX;



/* --------------------------------------------- Macros */

/* --------------------------------------------- APIs */
GA_RESULT ga_brr_get_module_pid(UINT32 id, GA_BRR_PRF_HANDLE * pid);

GA_RESULT ga_brr_int_cb
          (
              /* IN */ GA_BRR_DEVICE * device,
              /* IN */ void          * context,
              /* IN */ UCHAR         brr_role,
              /* IN */ void          * inst_handle,
              /* IN */ UINT8         event_type,
              /* IN */ UINT16        event_status,
              /* IN */ void          * event_data,
              /* IN */ UINT16        event_datalen
          );

/** GA Bearer Internal Initialization */
GA_RESULT ga_brr_int_init(void);

/** Bearer interface function declarations */
GA_RESULT ga_brr_init(GA_BRR_CB cb);
GA_RESULT ga_brr_shutdown(void);

GA_RESULT ga_brr_add_service
          (
              /* IN  */ GA_BRR_UUID service_uuid,
              /* IN  */ UINT8 service_type,
              /* IN  */ GA_BRR_SERVICE_CB cb,
              /* OUT */ GA_BRR_SVC_INST * sid
          );
GA_RESULT ga_brr_remove_service
          (
              /* IN  */ GA_BRR_SVC_INST sid
          );
GA_RESULT ga_brr_include_service
          (
              /* IN  */ GA_BRR_SVC_INST sid,
              /* IN  */ GA_BRR_UUID service_uuid,
              /* IN  */ UINT8 service_state,
              /* OUT */ GA_BRR_SVC_INST * isid
          );
GA_RESULT ga_brr_add_characteristic
          (
              /* IN  */ GA_BRR_SVC_INST sid,
              /* IN  */ GA_BRR_UUID     char_uuid,
              /* IN  */ UINT16          char_property,
              /* IN  */ GA_BRR_CHR_VAL  * value,
              /* OUT */ GA_BRR_CHR_INST * cid
          );
GA_RESULT ga_brr_manage_service
          (
              /* IN  */ UINT8 sid,
              /* IN  */ UINT8 iid,
              /* IN  */ UINT8 state
          );
GA_RESULT ga_brr_manage_characteristic
          (
              /* IN  */ UINT8 sid,
              /* IN  */ UINT8 cid,
              /* IN  */ UINT8 state
          );
GA_RESULT ga_brr_write_characteristic_rsp
          (
              /* IN */ GA_BRR_DEVICE      * device,
              /* IN */ GA_BRR_CHR_CONTEXT * ctx,
              /* IN */ UINT16             status
          );
GA_RESULT ga_brr_read_characteristic_rsp
          (
              /* IN */ GA_BRR_DEVICE      * device,
              /* IN */ GA_BRR_CHR_CONTEXT * ctx,
              /* IN */ UINT16             status,
              /* IN */ GA_BRR_CHR_VAL     * value
          );
GA_RESULT ga_brr_notify_characteristic
          (
              /* IN */ GA_BRR_DEVICE   * device,
              /* IN */ GA_BRR_SVC_INST sid,
              /* IN */ GA_BRR_UUID     char_uuid,
              /* IN */ GA_BRR_CHR_INST cid,
              /* IN */ GA_BRR_CHR_VAL  * value,
              /* IN */ UINT8           nwr
          );
GA_RESULT ga_brr_setup_transport
          (
              /* IN */ GA_BRR_PRF_HANDLE pid,
              /* IN */ UINT8 type,
              /* IN */ UINT8 enable
          );
GA_RESULT ga_brr_discover_service
          (
              /* IN */ GA_BRR_PRF_HANDLE pid,
              /* IN */ GA_BRR_DEVICE     * device,
              /* IN */ GA_BRR_SVC_INFO   * svc_info,
              /* IN */ UCHAR             disc_type,
              /* IN */ void              * context
          );
GA_RESULT ga_brr_get_service_composition
          (
              /* IN */ GA_BRR_PRF_HANDLE   pid,
              /* IN */ GA_BRR_DEVICE     * device,
              /* IN */ GA_BRR_SVC_INFO   * service,
              /* IN */ void              * context
          );
GA_RESULT ga_brr_get_characteristic_composition
          (
              /* IN */ GA_BRR_PRF_HANDLE   pid,
              /* IN */ GA_BRR_DEVICE     * device,
              /* IN */ GA_BRR_CHAR_RANGE * characteristic,
              /* IN */ void              * context
          );
GA_RESULT ga_brr_read_characteristic
          (
              /* IN */ GA_BRR_PRF_HANDLE pid,
              /* IN */ GA_BRR_DEVICE     * device,
              /* IN */ GA_BRR_CHR_HANDLE characteristic,
              /* IN */ void              * context
          );
GA_RESULT ga_brr_write_characteristic
          (
              /* IN */ GA_BRR_PRF_HANDLE pid,
              /* IN */ GA_BRR_DEVICE     * device,
              /* IN */ GA_BRR_CHR_HANDLE characteristic,
              /* IN */ GA_BRR_CHR_VAL    * value,
              /* IN */ UINT8             wwr,
              /* IN */ void              * context
          );
GA_RESULT ga_brr_config_notification
          (
              /* IN */ GA_BRR_PRF_HANDLE pid,
              /* IN */ GA_BRR_DEVICE     * device,
              /* IN */ GA_BRR_CHR_HANDLE characteristic,
              /* IN */ UINT8             state,
              /* IN */ void              * context
          );
GA_RESULT ga_brr_register_profile
          (
              /* IN  */ UINT32 id,
              /* IN  */ GA_BRR_PROFILE_CB cb,
              /* OUT */ GA_BRR_PRF_HANDLE * pid
          );
GA_RESULT ga_brr_unregister_profile
          (
              /* IN */ GA_BRR_PRF_HANDLE pid
          );
GA_RESULT ga_brr_dispatch_event
          (
              /* IN */ GA_BRR_PRF_HANDLE        pid,
              /* IN */ GA_BRR_DEVICE           *device,
              /* IN */ GA_BRR_EVENT_PARAMS     *event_params,
              /* IN */ void                    *context
          );

#endif /* _H_GA_BRR_INTERNAL_ */

