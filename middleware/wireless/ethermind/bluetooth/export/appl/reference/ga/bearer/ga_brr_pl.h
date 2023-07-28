
/**
 *  \file ga_brr_pl.h
 *
 *
 */

/*
 *  Copyright (C) 2020. Mindtree Limited.
 *  All rights reserved.
 */

#ifndef _H_GA_BRR_PL_
#define _H_GA_BRR_PL_
/* --------------------------------------------- Header File Inclusion */
/* GA Bearer Related Includes */
#include "GA_bearer_api.h"
#include "BT_gatt_db_api.h"
#include "gatt_db_pl.h"
#include "write_task.h"

/* --------------------------------------------- Global Definitions */
/** TODO :
 * Move this section to some other location
 */
#if 0
#define GA_BRR_PL_ERR(...) printf(__VA_ARGS__)
#define GA_BRR_PL_TRC(...) printf(__VA_ARGS__)
#define GA_BRR_PL_INF(...) printf(__VA_ARGS__)
#else /* 0 */
#define GA_BRR_PL_ERR(...)               EM_debug_error(BT_MODULE_ID_COMMON,__VA_ARGS__)
#define GA_BRR_PL_TRC(...)               EM_debug_trace(BT_MODULE_ID_COMMON,__VA_ARGS__)
#define GA_BRR_PL_INF(...)               EM_debug_info(BT_MODULE_ID_COMMON,__VA_ARGS__)
#define GA_brr_pl_debug_dump_bytes(b,l)  EM_debug_dump_bytes(BT_MODULE_ID_COMMON, (b), (l))
#endif /* 0 */

/* ============================= Limits ============================== */
#define GA_BRR_MAX_DEVICE_BRR_LIST_COUNT                      0x05U
#define GA_BRR_MAX_ACTIVITY_CNTX                              0x0AU

#define GA_BRR_DEVICE_INVALID_MTU                             0x0000U
#define GA_BRR_DEVICE_DEFAULT_MTU                             64U

/* --------------------------------------------- Structures/Data Types */
typedef struct _GA_BRR_EXTENDED_SCAN_PARAMS
{
    UINT8  own_addr_type;
    UINT8  scan_filter_policy;
    UINT8  scan_phy;
    UINT8  scan_type;
    UINT16 scan_interval;
    UINT16 scan_window;
    UINT8  filter_duplicates;
    UINT16 duration;
    UINT16 period;
}GA_BRR_EXTENDED_SCAN_PARAMS;

typedef struct _GA_BRR_PERIODIC_SYNC_PARAMS
{
    UINT8   options;
    UINT16  skip;
    UINT16  sync_timeout;
    UINT8   sync_cte_type;
}GA_BRR_PERIODIC_SYNC_PARAMS;

typedef struct _GA_BRR_PERIODIC_ADV_PARAMS
{
    UINT16  per_adv_interval_min;
    UINT16  per_adv_interval_max;
    UINT16  per_adv_prty;
}GA_BRR_PERIODIC_ADV_PARAMS;

typedef struct _GA_BRR_EXTENDED_ADV_PARAMS
{
    UINT16  adv_event_properties;
    UINT32  pri_adv_interval_min;
    UINT32  pri_adv_interval_max;
    UINT8   pri_adv_channel_map;
    UINT8   own_addr_type;
    UINT8   peer_addr_type;
    UINT8   peer_addr[BT_BD_ADDR_SIZE];
    UINT8   adv_filter_policy;
    INT8    adv_tx_power;
    UINT8   pri_adv_phy;
    UINT8   sec_adv_max_skip;
    UINT8   sec_adv_phy;
    UINT8   scan_req_nfy_enable;
    UINT16  duration;
    UINT8   max_ext_adv_evts;
    UINT8   frag_pref;
} GA_BRR_EXTENDED_ADV_PARAMS;

typedef struct _GA_BRR_BROADCAST_SYNC_PARAMS
{
    UINT8   mse;
    UINT16  sync_timeout;
}GA_BRR_BROADCAST_SYNC_PARAMS;

typedef struct _GA_BRR_DISPATCH_BH_PARAMS
{
    GA_BRR_PRF_HANDLE        pid;
    GA_BRR_DEVICE            device;
    GA_BRR_EVENT_PARAMS      event_params;
    void                    *context;
} GA_BRR_DISPATCH_BH_PARAMS;

/* --------------------------------------------- Macros */

/* --------------------------------------------- Internal Functions */

/* --------------------------------------------- API Declarations */
/* ----------------------------------------------------------------------- */
/* ======================General - Role Agnostic========================== */
/* ----------------------------------------------------------------------- */
void ga_brr_register(void);

GA_RESULT ga_brr_init_pl(void);
GA_RESULT ga_brr_register_pl_cb(GA_BRR_PL_CB cb);
GA_RESULT ga_brr_shutdown_pl(void);

/* ----------------------------------------------------------------------- */
/* =========================Profile - Related============================= */
/* ----------------------------------------------------------------------- */
GA_RESULT ga_brr_discover_service_pl
          (
              /* IN */ GA_BRR_PRF_HANDLE pid,
              /* IN */ GA_BRR_DEVICE     * device,
              /* IN */ GA_BRR_SVC_INFO   * svc_info,
              /* IN */ UCHAR             disc_type,
              /* IN */ void              * context
          );
GA_RESULT ga_brr_get_service_composition_pl
          (
              /* IN */ GA_BRR_PRF_HANDLE   pid,
              /* IN */ GA_BRR_DEVICE     * device,
              /* IN */ GA_BRR_SVC_INFO   * service,
              /* IN */ void              * context
          );
GA_RESULT ga_brr_get_characteristic_composition_pl
          (
              /* IN */ GA_BRR_PRF_HANDLE   pid,
              /* IN */ GA_BRR_DEVICE     * device,
              /* IN */ GA_BRR_CHAR_RANGE * characteristic,
              /* IN */ void              * context
          );
GA_RESULT ga_brr_read_characteristic_pl
          (
              /* IN */ GA_BRR_PRF_HANDLE pid,
              /* IN */ GA_BRR_DEVICE     * device,
              /* IN */ GA_BRR_CHR_HANDLE characteristic,
              /* IN */ void              * context
          );
GA_RESULT ga_brr_write_characteristic_pl
          (
              /* IN */ GA_BRR_PRF_HANDLE pid,
              /* IN */ GA_BRR_DEVICE     * device,
              /* IN */ GA_BRR_CHR_HANDLE characteristic,
              /* IN */ GA_BRR_CHR_VAL    * value,
              /* IN */ UINT8             wwr,
              /* IN */ void              * context
          );
GA_RESULT ga_brr_config_notification_pl
          (
              /* IN */ GA_BRR_PRF_HANDLE pid,
              /* IN */ GA_BRR_DEVICE     * device,
              /* IN */ GA_BRR_CHR_HANDLE characteristic,
              /* IN */ UINT8             state,
              /* IN */ void              * context
          );
GA_RESULT ga_brr_config_adv
          (
              GA_BRR_PRF_HANDLE pid,
              UINT8   type,
              UINT8   handle,
              UINT8   enable,
              UCHAR * data,
              UINT8   data_len,
              void  * context
          );
GA_RESULT ga_brr_config_scan
          (
              GA_BRR_PRF_HANDLE   pid,
              UCHAR               enable,
              void              * context
          );
GA_RESULT ga_brr_config_sync
          (
              GA_BRR_PRF_HANDLE pid,
              UINT16            handle,
              GA_BRR_DEVICE   * device,
              void            * context
          );
GA_RESULT ga_brr_config_broadcast
          (
              GA_BRR_PRF_HANDLE    pid,
              UINT8                handle,
              UINT8                adv_handle,
              GA_BRR_BCAST_PARAM * param,
              void               * context
          );
GA_RESULT ga_brr_config_broadcast_sync
          (
              GA_BRR_PRF_HANDLE         pid,
              UINT8                     handle,
              UINT16                    sync_handle,
              GA_BRR_BCAST_SYNC_PARAM * sync_params,
              void                    * context
          );
void ga_brr_set_default_broadcast_settings(void);

/* ----------------------------------------------------------------------- */
/* =========================Service - Related============================= */
/* ----------------------------------------------------------------------- */
GA_RESULT ga_brr_add_service_pl
          (
              /* IN  */ GA_BRR_UUID     service_uuid,
              /* IN  */ UINT8           service_type,
              /* IN */  UCHAR           lkid,
              /* OUT */ GA_BRR_SVC_INST * sid
          );
GA_RESULT ga_brr_include_service_add_pl
          (
              /* IN  */ GA_BRR_SVC_INST sid,
              /* IN  */ GA_BRR_UUID     service_uuid,
              /* IN  */ UINT8           service_state,
              /* IN */  UCHAR           lkid,
              /* OUT */ GA_BRR_SVC_INST * isid
          );
GA_RESULT ga_brr_add_characteristic_pl
          (
              /* IN  */ GA_BRR_SVC_INST sid,
              /* IN  */ GA_BRR_UUID     char_uuid,
              /* IN  */ UINT16          char_property,
              /* IN  */ GA_BRR_CHR_VAL  * value,
              /* IN */  UCHAR           lkid,
              /* OUT */ GA_BRR_CHR_INST * cid
          );
GA_RESULT ga_brr_manage_service_pl
          (
              /* IN  */ UINT8 sid,
              /* IN  */ UINT8 iid,
              /* IN  */ UINT8 state
          );
GA_RESULT ga_brr_manage_characteristic_pl
          (
              /* IN  */ UINT8 sid,
              /* IN  */ UINT8 cid,
              /* IN  */ UINT8 state
          );
GA_RESULT ga_brr_update_characteristic_value_pl
          (
              /* IN */ GA_BRR_SVC_INST sid,
              /* IN */ GA_BRR_UUID     char_uuid,
              /* IN */ GA_BRR_CHR_INST cid,
              /* IN */ GA_BRR_CHR_VAL  * value
          );
GA_RESULT ga_brr_write_characteristic_rsp_pl
          (
              /* IN */ GA_BRR_DEVICE      * device,
              /* IN */ GA_BRR_CHR_CONTEXT * ctx,
              /* IN */ UINT16             status
          );
GA_RESULT ga_brr_read_characteristic_rsp_pl
          (
              /* IN */ GA_BRR_DEVICE      * device,
              /* IN */ GA_BRR_CHR_CONTEXT * ctx,
              /* IN */ UINT16             status,
              /* IN */ GA_BRR_CHR_VAL     * value
          );
GA_RESULT ga_brr_notify_characteristic_pl
          (
              /* IN */ GA_BRR_DEVICE   * device,
              /* IN */ GA_BRR_SVC_INST sid,
              /* IN */ GA_BRR_UUID     char_uuid,
              /* IN */ GA_BRR_CHR_INST cid,
              /* IN */ GA_BRR_CHR_VAL  * value,
              /* IN */ UINT8           nwr
          );
GA_RESULT ga_brr_dispatch_event_pl
          (
              /* IN */ GA_BRR_PRF_HANDLE        pid,
              /* IN */ GA_BRR_DEVICE           *device,
              /* IN */ GA_BRR_EVENT_PARAMS     *event_params,
              /* IN */ void                    *context
          );
void ga_brr_dispatch_event_handler
     (
         /* IN */ GA_BRR_PRF_HANDLE        pid,
         /* IN */ GA_BRR_DEVICE           *device,
         /* IN */ GA_BRR_EVENT_PARAMS     *event_params,
         /* IN */ void                    *context
     );
API_RESULT gatt_db_ga_gatt_char_handler
           (
               GATT_DB_HANDLE * handle,
               GATT_DB_PARAMS * params
           );

GA_RESULT ga_brr_dyn_gatt_db_init_pl
          (
              void * db_blob
          );
#endif /* _H_GA_BRR_PL_ */


