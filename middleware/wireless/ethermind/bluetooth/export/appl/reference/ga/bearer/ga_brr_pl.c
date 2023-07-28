
/**
*  \file ga_brr_pl.c
*
*  \brief This file defines the Generic Audio Bearer Platform Abstractions,
*  Data Structures and Methods.
*/

/*
*  Copyright (C) 2020. Mindtree Ltd.
*  All rights reserved.
*/

/* --------------------------------------------- Header File Inclusion */
#include "ga_brr_internal.h"

/* For appl_gatt_set_mtu() */
#include "appl_gatt_server.h"

/* For HCI Events */
#include "appl_hci.h"

#ifdef BT_GAM

/* EtherMind Stack related Includes */
/**
 * NOTE:
 * These files are included here as other GA BRR files
 * need not be aware of it.
 */
/* Common Stack Header */
#include "BT_common.h"
/* Device Queue Header */
#include "BT_device_queue.h"
/* HCI Header */
#include "BT_hci_api.h"
/* ATT related Header */
#include "BT_att_api.h"
/* Gatt Module Header */
#include "gatt.h"
/* GATT DB Module Header */
#include "BT_gatt_db_api.h"
/* Global GATT assigned Number Header */
#include "gatt_defines.h"

#ifdef BT_DUAL_MODE
/* SDP related Database Header */
#include "db_gen.h"
#endif /* BT_DUAL_MODE */

/* Application GATT Database */
#include "ga_gatt_db.h"


/**
 * Have this enabled to have Context Busy check.
 * Currently this is disabled.
 * This is not needed in scenarios where UL invokes
 * procedures from the callback context, and also when
 * the exisiting corresponding activity context is intact
 * and in-need for some pending operations etc.
 */
/* #define GA_BRR_HAVE_CHK_ACT_CTX_BUSY */

/* Global mutex variable for BRR */
GA_DEFINE_MUTEX(brr_mutex)

/* --------------------------------------------- External Global Variables */
/** TO REMOVE LATER */
extern void appl_dump_bytes (UCHAR *buffer, UINT16 length);
extern void gatt_db_register_ga(void);
extern API_RESULT appl_set_gatt_service_in_sdp_record
                  (
                      UCHAR service_instance,
                      UCHAR sdp_instance
                  );
/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Structures/Data Types */
typedef struct _GA_BRR_DEVICE_BRR_LIST_PL_
{
    /**
     * Bearer Devive
     */
    GA_BRR_DEVICE brr_dev;

    /**
     * Profile ID
     */
    ATT_HANDLE    att_handle;

    /**
     * ATT Bearer MTU.
     */
    UINT16        att_mtu;
}GA_BRR_DEVICE_BRR_LIST_PL;

typedef struct _GA_BRR_ACTIVITY_CNTX_PL_
{
    /**
     * Profile ID context
     */
    GA_BRR_PRF_HANDLE pid;

    /**
     * Profile provided Context
     */
    void              * context;

    /**
     * Current state of the Profile
     * BT_TRUE : Currently Busy
     * BT_FALSE: Currently Not Busy
     */
    UCHAR             busy;

    /**
     * Additionl Bearer specific meta info for Look ups
     */
    void              * meta_info;

    /**
     * Any Expected specific event by the profile
     */
    UCHAR             expt_event;

    /**
     * Process on which the profile is waiting for
     */
    UCHAR             process;

    /**
     * GA Bearer Device End point
     */
    GA_BRR_DEVICE     dev;

}GA_BRR_ACTIVITY_CNTX_PL;

/* --------------------------------------------- Static Global Variables */
/**
 * Global Call Back to Inform GA BRR Internal layer from GA BRR
 * Platform Abstraction
 */
DECL_STATIC GA_BRR_PL_CB  ga_brr_pl_cb;

/**
 * List of Peer Device and corresponding ATT Instance Mapping
 */
DECL_STATIC GA_BRR_DEVICE_BRR_LIST_PL ga_brr_dev_list_pl[GA_BRR_MAX_DEVICE_BRR_LIST_COUNT];

/**
 * List of Acitivity contexts
 */
DECL_STATIC GA_BRR_ACTIVITY_CNTX_PL ga_brr_act_ctx[GA_BRR_MAX_ACTIVITY_CNTX];

/**
 * Scan/Adv parameters
 */
DECL_STATIC GA_BRR_EXTENDED_SCAN_PARAMS     ga_brr_escan_params;
DECL_STATIC GA_BRR_EXTENDED_ADV_PARAMS      ga_brr_eadv_params;
DECL_STATIC GA_BRR_PERIODIC_SYNC_PARAMS     ga_brr_psync_params;
DECL_STATIC GA_BRR_PERIODIC_ADV_PARAMS      ga_brr_padv_params;
DECL_STATIC GA_BRR_BROADCAST_SYNC_PARAMS    ga_brr_bsync_params;

/* GA CAP initialization identifier */
DECL_STATIC GA_DEFINE_MODULE_STATE(brr)

/* Lock and Unlock Macros */
#define BRR_LOCK()                  GA_MUTEX_LOCK(brr_mutex, BRR)
#define BRR_LOCK_VOID()             GA_MUTEX_LOCK_VOID(brr_mutex, BRR)
#define BRR_UNLOCK()                GA_MUTEX_UNLOCK(brr_mutex, BRR)
#define BRR_UNLOCK_VOID()           GA_MUTEX_UNLOCK_VOID(brr_mutex, BRR)

/* --------------------------------------------- Internal Functions */
/* ----------------------------------------------------------------------- */
/* ===================Internal Function Prototypes======================== */
/* ----------------------------------------------------------------------- */
API_RESULT ga_get_bd_addr_from_dev_hndl
           (
               DEVICE_HANDLE  * hndl,
               BT_DEVICE_ADDR * bd_addr
           );
API_RESULT ga_get_ga_dev_from_bd_addr
           (
               void          * bd_addr,
               void          * ext_id,
               GA_BRR_DEVICE * ga_dev
           );

GA_RESULT ga_hci_event_callback
          (
              UINT8   event_code,
              UINT8 * event_data,
              UINT8 event_datalen
          );

API_RESULT ga_gatt_cb_pl
           (
               ATT_HANDLE    * handle,
               UCHAR         att_event,
               API_RESULT    event_result,
               void          * eventdata,
               UINT16        event_datalen
           );

GA_RESULT ga_brr_search_dev_list_by_handle_pl
          (
              /* IN  */ ATT_HANDLE handle,
              /* OUT */ UCHAR      * id
          );

GA_RESULT ga_brr_search_dev_list_by_dev_pl
          (
              /* IN  */ GA_BRR_DEVICE * ga_dev,
              /* OUT */ UCHAR         * id
          );

GA_RESULT ga_brr_add_to_dev_list_pl
          (
              /* IN  */ GA_BRR_DEVICE * ga_dev,
              /* IN  */ ATT_HANDLE    handle,
              /* OUT */ UCHAR         * id
          );

GA_RESULT ga_brr_remove_from_dev_list_by_dev_pl
          (
              /* IN  */ GA_BRR_DEVICE * ga_dev
          );

GA_RESULT ga_brr_remove_from_dev_list_by_handle_pl
          (
              /* IN  */ ATT_HANDLE handle
          );


/* Activity Context Related */
GA_RESULT ga_brr_find_activity_by_pid_pl
          (
              /* IN  */ GA_BRR_PRF_HANDLE pid,
              /* OUT */ UCHAR             * id
          );

GA_RESULT ga_brr_find_activity_by_dev_pl
          (
              /* IN  */ GA_BRR_DEVICE * ga_dev,
              /* OUT */ UCHAR         * id
          );

GA_RESULT ga_brr_find_activity_pl
          (
              /* OUT */ UCHAR * id
          );

GA_RESULT ga_brr_add_to_activity_pl
          (
              /* IN  */ GA_BRR_PRF_HANDLE pid,
              /* IN  */ GA_BRR_DEVICE     * ga_dev,
              /* IN  */ void              * context,
              /* OUT */ UCHAR             * id
          );

GA_RESULT ga_brr_delete_activity_pl
          (
              /* IN  */ GA_BRR_PRF_HANDLE pid
          );

/* ----------------------------------------------------------------------- */
/* ===================Internal Function Definitions======================= */
/* ----------------------------------------------------------------------- */
GA_RESULT ga_get_bd_addr_from_dev_hndl
           (
               DEVICE_HANDLE  * hndl,
               BT_DEVICE_ADDR * bd_addr
           )
{
    GA_RESULT retval;

    retval = GA_SUCCESS;

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: >> ga_get_bd_addr_from_dev_hndl\n");

    /* Get BD Address from DEV Handle */
    retval = device_queue_get_remote_addr
             (
                 hndl,
                 bd_addr
             );

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: << ga_get_bd_addr_from_dev_hndl, retval 0x%04X\n", retval);

    return retval;
}

GA_RESULT ga_get_ga_dev_from_bd_addr
           (
               void          * bd_addr,
               void          * ext_id,
               GA_BRR_DEVICE * ga_dev
           )
{
    GA_RESULT     retval;
    BT_DEVICE_ADDR * b;
    ATT_HANDLE     * a;

    retval = GA_SUCCESS;

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: >> ga_get_ga_dev_from_bd_addr\n");

    /**
     * This parameter currently not used and this is kept in place for
     * Multiple ATT Bearer Scenario.
     */
    if (NULL != ext_id)
    {
        a = (ATT_HANDLE *)ext_id;
    }
    b = (BT_DEVICE_ADDR *)bd_addr;

    BT_COPY_BD_ADDR(ga_dev->bd_addr, b->addr);
    ga_dev->bd_type = b->type;

    BT_IGNORE_UNUSED_PARAM(a);

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: << ga_get_ga_dev_from_bd_addr, retval 0x%04X\n", retval);
    return retval;
}

GA_RESULT ga_brr_search_dev_list_by_handle_pl
          (
              /* IN  */ ATT_HANDLE handle,
              /* OUT */ UCHAR      * id
          )
{
    UINT32 i;
    GA_RESULT retval;

    retval = GA_FAILURE;

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: >> ga_brr_search_dev_list_by_handle_pl\n");

    *id = GA_BRR_MAX_DEVICE_BRR_LIST_COUNT;

    for (i = 0; i < GA_BRR_MAX_DEVICE_BRR_LIST_COUNT; i++)
    {
        if ((ga_brr_dev_list_pl[i].att_handle.att_id == handle.att_id) &&
            (ga_brr_dev_list_pl[i].att_handle.device_id == handle.device_id))
        {
            *id = (UCHAR)i;
            retval = GA_SUCCESS;

            break;
        }
    }

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: << ga_brr_search_dev_list_by_handle_pl, retval 0x%04X\n", retval);

    return retval;
}

GA_RESULT ga_brr_search_dev_list_by_dev_pl
          (
              /* IN  */ GA_BRR_DEVICE * ga_dev,
              /* OUT */ UCHAR         * id
          )
{
    UINT32 i;
    GA_RESULT retval;

    retval = GA_FAILURE;

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: >> ga_brr_search_dev_list_by_dev_pl\n");

    *id = GA_BRR_MAX_DEVICE_BRR_LIST_COUNT;

    if (GA_BRR_DEV_ADDR_IS_NON_ZERO(ga_dev->bd_addr))
    {
        for (i = 0; i < GA_BRR_MAX_DEVICE_BRR_LIST_COUNT; i++)
        {
            if (GA_COMPARE_BRR_DEVICE(ga_dev, &ga_brr_dev_list_pl[i].brr_dev))
            {
                *id = (UCHAR)i;
                retval = GA_SUCCESS;

                break;
            }
        }
    }

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: << ga_brr_search_dev_list_by_dev_pl, retval 0x%04X\n", retval);

    return retval;
}

GA_RESULT ga_brr_add_to_dev_list_pl
          (
              /* IN  */ GA_BRR_DEVICE * ga_dev,
              /* IN  */ ATT_HANDLE    handle,
              /* OUT */ UCHAR         * id
          )
{
    GA_RESULT retval;
    UCHAR     i;

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: >> ga_brr_add_to_dev_list_pl\n");

    /* Param Check */
    if (NULL == id)
    {
        GA_BRR_PL_ERR (
        "[GA_BRR_PL]: Invalid Parameters!\n");

        return  GA_BRR_INVALID_PARAMS;
    }

    /* Init */
    retval = GA_FAILURE;
    *id = GA_BRR_MAX_DEVICE_BRR_LIST_COUNT;

    for (i = 0; i < GA_BRR_MAX_DEVICE_BRR_LIST_COUNT; i++)
    {
        if ((GA_BRR_DEVICE_INVALID_MTU == ga_brr_dev_list_pl[i].att_mtu) &&
            !(GA_BRR_DEV_ADDR_IS_NON_ZERO(ga_brr_dev_list_pl[i].brr_dev.bd_addr)))
        {
            *id = i;
            retval = GA_SUCCESS;

            /* Store the incoming values */
            ga_brr_dev_list_pl[i].att_handle = handle;
            ga_brr_dev_list_pl[i].att_mtu    = GA_BRR_DEVICE_DEFAULT_MTU;
            GA_COPY_BRR_DEVICE
            (
                &ga_brr_dev_list_pl[i].brr_dev,
                ga_dev
            );

            break;
        }
    }

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: << ga_brr_add_to_dev_list_pl, retval 0x%04X\n", retval);

    return retval;
}

GA_RESULT ga_brr_remove_from_dev_list_by_dev_pl
          (
              /* IN  */ GA_BRR_DEVICE * ga_dev
          )
{
    GA_RESULT retval;
    UCHAR     id;

    retval = GA_FAILURE;

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: >> ga_brr_remove_from_dev_list_by_dev_pl\n");

    /* Reset */
    if (GA_SUCCESS == ga_brr_search_dev_list_by_dev_pl(ga_dev, &id))
    {
        BT_mem_set
        (
            &ga_brr_dev_list_pl[id],
            0x0,
            sizeof(GA_BRR_DEVICE_BRR_LIST_PL)
        );
        ATT_INIT_HANDLE(ga_brr_dev_list_pl[id].att_handle);
        ga_brr_dev_list_pl[id].att_mtu    = GA_BRR_DEVICE_INVALID_MTU;
    }

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: << ga_brr_remove_from_dev_list_by_dev_pl, retval 0x%04X\n", retval);

    return retval;
}

GA_RESULT ga_brr_remove_from_dev_list_by_handle_pl
          (
              /* IN  */ ATT_HANDLE handle
          )
{
    GA_RESULT retval;
    UCHAR     id;

    retval = GA_SUCCESS;

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: >> ga_brr_remove_from_dev_list_by_handle_pl\n");

    /* Reset */
    if (GA_SUCCESS == ga_brr_search_dev_list_by_handle_pl(handle, &id))
    {
        BT_mem_set
        (
            &ga_brr_dev_list_pl[id],
            0x0,
            sizeof(GA_BRR_DEVICE_BRR_LIST_PL)
        );
        ATT_INIT_HANDLE(ga_brr_dev_list_pl[id].att_handle);
        ga_brr_dev_list_pl[id].att_mtu    = GA_BRR_DEVICE_INVALID_MTU;
    }

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: << ga_brr_remove_from_dev_list_by_handle_pl, retval 0x%04X\n", retval);

    return retval;
}

GA_RESULT ga_brr_find_activity_by_pid_pl
          (
              /* IN  */ GA_BRR_PRF_HANDLE pid,
              /* OUT */ UCHAR             * id
          )
{
    UINT32 i;
    GA_RESULT retval;

    retval = GA_FAILURE;

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: >> ga_brr_find_activity_by_pid_pl\n");

    *id = GA_BRR_MAX_ACTIVITY_CNTX;

    for (i = 0; i < GA_BRR_MAX_ACTIVITY_CNTX; i++)
    {
        if (ga_brr_act_ctx[i].pid == pid)
        {
            *id = (UCHAR)i;
            retval = GA_SUCCESS;

            break;
        }
    }

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: << ga_brr_find_activity_by_pid_pl, retval 0x%04X\n", retval);

    return retval;
}

GA_RESULT ga_brr_find_activity_by_dev_pl
          (
              /* IN  */ GA_BRR_DEVICE * ga_dev,
              /* OUT */ UCHAR         * id
          )
{
    UINT32 i;
    GA_RESULT retval;

    retval = GA_FAILURE;

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: >> ga_brr_find_activity_by_dev_pl\n");

    *id = GA_BRR_MAX_ACTIVITY_CNTX;

    for (i = 0; i < GA_BRR_MAX_ACTIVITY_CNTX; i++)
    {
        /**
         * Find if the BRR DEVICE entity Matches and also
         * if the state associated with it is set to "Busy: BT_TRUE"
         */
        if ((GA_COMPARE_BRR_DEVICE(ga_dev, &ga_brr_act_ctx[i].dev)) &&
            (BT_TRUE == ga_brr_act_ctx[i].busy))
        {
            *id = (UCHAR)i;
            retval = GA_SUCCESS;

            break;
        }
    }

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: << ga_brr_find_activity_by_dev_pl, retval 0x%04X\n", retval);

    return retval;
}

GA_RESULT ga_brr_find_activity_pl
          (
              /* OUT */ UCHAR * id
          )
{
    UINT32 i;
    GA_RESULT retval;

    retval = GA_FAILURE;

    GA_BRR_PL_TRC(
        "[GA_BRR_PL]: >> ga_brr_find_activity_pl\n");

    *id = GA_BRR_MAX_ACTIVITY_CNTX;

    for (i = 0; i < GA_BRR_MAX_ACTIVITY_CNTX; i++)
    {
        /**
         * Fetch the Profile and validate the state associated with it is set
         * to "Busy: BT_TRUE"
         */

        /* TODO: What if there are multi profiles which are busy */
        if (BT_TRUE == ga_brr_act_ctx[i].busy)
        {
            *id = (UCHAR)i;
            retval = GA_SUCCESS;

            break;
        }
    }

    GA_BRR_PL_TRC(
        "[GA_BRR_PL]: << ga_brr_find_activity_pl, retval 0x%04X\n", retval);

    return retval;
}

GA_RESULT ga_brr_add_to_activity_pl
          (
              /* IN  */ GA_BRR_PRF_HANDLE pid,
              /* IN  */ GA_BRR_DEVICE     * ga_dev,
              /* IN  */ void              * context,
              /* OUT */ UCHAR             * id
          )
{
    GA_RESULT retval;
    UCHAR     i;

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: >> ga_brr_add_to_activity_pl\n");

    /* Param Check */
    if (NULL == id)
    {
        GA_BRR_PL_ERR (
        "[GA_BRR_PL]: Invalid Parameters!\n");

        return  GA_BRR_INVALID_PARAMS;
    }

    /* Init */
    retval = GA_FAILURE;

    for (i = 0; i < GA_BRR_MAX_ACTIVITY_CNTX; i++)
    {
        if (GA_BRR_PROFILE_ID_INVALID == ga_brr_act_ctx[i].pid)
        {
            *id = i;
            retval = GA_SUCCESS;

            /* Store the incoming values */
            ga_brr_act_ctx[i].pid     = pid;
            ga_brr_act_ctx[i].context = context;

            if (NULL != ga_dev)
            {
                GA_COPY_BRR_DEVICE
                (
                    &ga_brr_act_ctx[i].dev,
                    ga_dev
                );
            }

            break;
        }
    }
    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: << ga_brr_add_to_activity_pl, retval 0x%04X\n", retval);

    return retval;
}

GA_RESULT ga_brr_delete_activity_pl
          (
              /* IN  */ GA_BRR_PRF_HANDLE pid
          )
{
    GA_RESULT retval;
    UCHAR     id;

    retval = GA_FAILURE;

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: >> ga_brr_delete_activity_pl\n");

    /* Reset */
    if (GA_SUCCESS == ga_brr_find_activity_by_pid_pl(pid, &id))
    {
        ga_brr_act_ctx[id].pid        = GA_BRR_PROFILE_ID_INVALID;
        ga_brr_act_ctx[id].context    = NULL;
        ga_brr_act_ctx[id].busy       = BT_FALSE;
        ga_brr_act_ctx[id].expt_event = GATT_INVALID_EVENT;
        ga_brr_act_ctx[id].process    = 0xFF;
        ga_brr_act_ctx[id].meta_info  = NULL;
        GA_INIT_BRR_DEVICE(&ga_brr_act_ctx[id].dev);
        retval                        = GA_SUCCESS;
    }

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: << ga_brr_delete_activity_pl, retval 0x%04X\n", retval);

    return retval;
}

/* --------------------------------------------- APIs */
/* ----------------------------------------------------------------------- */
/* ======================General - Role Agnostic========================== */
/* ----------------------------------------------------------------------- */
GA_RESULT ga_brr_init_pl(void)
{
    GA_RESULT retval;
    UCHAR     i;

    retval = GA_SUCCESS;

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: >> ga_brr_init_pl\n");

    /* Validate state */
    IF_GA_MODULE_STATE(brr, INITIALIZED)
    {
        GA_BRR_PL_ERR("[GA_BRR_PL]: Already Initialized !\n");
        return GA_FAILURE;
    }

    /* Validate state for Mutex Initialization */
    IF_GA_MODULE_STATE(brr, INVALID)
    {
        /* Initialize global GA BRR PL mutex */
        GA_MUTEX_INIT(brr_mutex, BRR);
    }

    /** Initialize Bearer Device List */
    for (i = 0; i < GA_BRR_MAX_DEVICE_BRR_LIST_COUNT; i++)
    {
        ATT_INIT_HANDLE(ga_brr_dev_list_pl[i].att_handle);
        ga_brr_dev_list_pl[i].att_mtu = GA_BRR_DEVICE_INVALID_MTU;
        GA_INIT_BRR_DEVICE(&ga_brr_dev_list_pl[i].brr_dev);
    }

    /** Initialize Bearer Activity Context */
    for (i = 0; i < GA_BRR_MAX_ACTIVITY_CNTX; i++)
    {
        ga_brr_act_ctx[i].pid        = GA_BRR_PROFILE_ID_INVALID;
        ga_brr_act_ctx[i].context    = NULL;
        ga_brr_act_ctx[i].busy       = BT_FALSE;
        ga_brr_act_ctx[i].expt_event = 0xFF;
        ga_brr_act_ctx[i].process    = 0xFF;
        ga_brr_act_ctx[i].meta_info = NULL;
        GA_INIT_BRR_DEVICE(&ga_brr_act_ctx[i].dev);
    }

    /* Update the MTU requirement of GA bearer */
    (BT_IGNORE_RETURN_VALUE)appl_gatt_set_mtu(GA_BRR_DEVICE_DEFAULT_MTU);

    /* If Static GATT Database is used, Register the DB here */
#ifndef GATT_DB_DYNAMIC
    /* Register the database */
    gatt_db_register_ga();
#endif /* GATT_DB_DYNAMIC */

    /* Register with HCI Appl for HCI Events */
    appl_hci_register_callback(&ga_hci_event_callback);

    /* Initialize default broadcast settings */
    ga_brr_set_default_broadcast_settings();

    /* Initialize the module state */
    GA_SET_MODULE_STATE(brr, INITIALIZED);

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: << ga_brr_init_pl, retval 0x%04X\n", retval);

    return retval;
}

GA_RESULT ga_brr_register_pl_cb(GA_BRR_PL_CB cb)
{
    GA_RESULT retval;

    retval = GA_FAILURE;

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: >> ga_brr_register_int_cb\n");

    if (NULL != cb)
    {
        ga_brr_pl_cb = cb;

        GA_BRR_PL_TRC("\nRegistered GA Internal Callback with GA Platform ABS!\n");

        retval = GA_SUCCESS;
    }

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: << ga_brr_register_int_cb, retval 0x%04X\n", retval);

    return retval;
}

GA_RESULT ga_brr_shutdown_pl(void)
{
    GA_RESULT retval;

    retval = GA_SUCCESS;

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: >> ga_brr_shutdown_pl\n");

    /* Validate state */
    IFN_GA_MODULE_STATE(brr, INITIALIZED)
    {
        GA_BRR_PL_ERR("[GA_BRR_PL]: Not Initialized !\n");
        return GA_FAILURE;
    }

    /* Register with HCI Appl for HCI Events */
    appl_hci_unregister_callback(&ga_hci_event_callback);

    /* De-Initialize global GA BRR PL mutex */
    GA_MUTEX_DEINIT(brr_mutex, BRR);

    /* Update the module state */
    GA_SET_MODULE_STATE(brr, SHUTDOWN);

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: << ga_brr_shutdown_pl, retval 0x%04X\n", retval);

    return retval;
}

void ga_brr_set_default_broadcast_settings (void)
{
    /* Initialize default broadcast scan parameters */
    ga_brr_escan_params.duration = 0x00;
    ga_brr_escan_params.filter_duplicates = 0x01;
    ga_brr_escan_params.own_addr_type = 0x00;
    ga_brr_escan_params.period = 0x00;
    ga_brr_escan_params.scan_filter_policy = 0x00;
    ga_brr_escan_params.scan_interval = 0x00A0;
    ga_brr_escan_params.scan_phy = 0x01;
    ga_brr_escan_params.scan_type = 0x00;
    ga_brr_escan_params.scan_window = 0x00A0;

    ga_brr_psync_params.options = 0x00;
    ga_brr_psync_params.skip = 0x00;
    ga_brr_psync_params.sync_cte_type = 0x00; /* ??? */
    ga_brr_psync_params.sync_timeout = 0x0C80;

    ga_brr_bsync_params.mse = 0x00;
    ga_brr_bsync_params.sync_timeout = 0x0C80;

    /* Initialize default broadcast adv parameters */
    ga_brr_eadv_params.adv_event_properties = 0x00;
    ga_brr_eadv_params.adv_filter_policy = 0x00;
    ga_brr_eadv_params.adv_tx_power = 0x00;
    ga_brr_eadv_params.duration = 0x00;
    ga_brr_eadv_params.frag_pref = 0x01;
    ga_brr_eadv_params.max_ext_adv_evts = 0x00;
    ga_brr_eadv_params.own_addr_type = 0x00;
    BT_mem_set(&ga_brr_eadv_params.peer_addr[0], 0, BT_BD_ADDR_SIZE);
    ga_brr_eadv_params.peer_addr_type = 0x00;
    ga_brr_eadv_params.pri_adv_channel_map = 0x07;
    ga_brr_eadv_params.pri_adv_interval_max = 0x00000020;
    ga_brr_eadv_params.pri_adv_interval_min = 0x00000020;
    ga_brr_eadv_params.pri_adv_phy = 0x01;
    ga_brr_eadv_params.scan_req_nfy_enable = 0x00;
    ga_brr_eadv_params.sec_adv_max_skip = 0x00;
    ga_brr_eadv_params.sec_adv_phy = 0x01;

    ga_brr_padv_params.per_adv_interval_max = 0x0320;
    ga_brr_padv_params.per_adv_interval_min = 0x0320;
    ga_brr_padv_params.per_adv_prty = 0x0000;
}

GA_RESULT ga_brr_config_adv
          (
              GA_BRR_PRF_HANDLE pid,
              UINT8   type,
              UINT8   handle,
              UINT8   enable,
              UCHAR * data,
              UINT8   datalen,
              void  * context
          )
{
    GA_RESULT retval;
    UCHAR     id;
    UCHAR     create_ctx;

    retval = GA_FAILURE;
    id = GA_BRR_MAX_ACTIVITY_CNTX;
    create_ctx = BT_TRUE;

    retval = GA_FAILURE;
    GA_IGNORE_UNUSED_PARAM(context);

    GA_BRR_PL_TRC(
    "[GA_BRR_PL]: >> ga_brr_config_adv\n");

    GA_BRR_PL_TRC(
    "[GA_BRR_PL]: Searching BRR PL Activity Context ...\n");

    /* Lock */
    BRR_LOCK();

    /**
     * Search for an existing Activity for this pid with the
     * state set to "Busy".
     */
    retval = ga_brr_find_activity_by_pid_pl(pid, &id);

    if (GA_SUCCESS == retval)
    {
        if (BT_TRUE == ga_brr_act_ctx[id].busy)
        {
            GA_BRR_PL_ERR(
            "[GA_BRR_PL]: Context exists for device at 0x%02X with operation"
            " for Profile ID 0x%02X\n", id, pid);

            /* Unlock */
            BRR_UNLOCK();

            return GA_FAILURE;
        }
    }

    /* Do not reuse the context even if it exists, Update the context */
    if (BT_TRUE == create_ctx)
    {
        retval = ga_brr_add_to_activity_pl
                 (
                     pid,
                     NULL,
                     context,
                     &id
                 );

        if (GA_SUCCESS != retval)
        {
            GA_BRR_PL_ERR(
            "[GA_BRR_PL]: Failed to create activity context for Profile ID"
            " 0x%02X\n", pid);

            /* Unlock */
            BRR_UNLOCK();

            return retval;
        }
    }

    if (BT_FALSE == enable)
    {
        /* Disable Periodic Adv if selected */
        if (GA_BRR_ADV_TYPE_PA & type)
        {
            retval = BT_hci_le_set_periodic_advertising_enable
                     (
                         0x00,
                         handle
                     );
        }

        /* Disable Extended Adv if selected */
        if (GA_BRR_ADV_TYPE_EA & type)
        {
            retval = BT_hci_le_set_extended_advertising_enable
                     (
                         0x00,
                         0x01,
                         &handle,
                         &ga_brr_eadv_params.duration,
                         &ga_brr_eadv_params.max_ext_adv_evts
                    );
        }
    }
    else
    {
        if (GA_BRR_ADV_TYPE_PA == type)
        {
            /* TODO: retval Needs to be GA specific */
            retval = BT_hci_le_set_periodic_advertising_parameters
                     (
                         handle,
                         ga_brr_padv_params.per_adv_interval_min,
                         ga_brr_padv_params.per_adv_interval_max,
                         ga_brr_padv_params.per_adv_prty
                     );

            if ((GA_SUCCESS == retval) && (NULL != data))
            {
                retval = BT_hci_le_set_periodic_advertising_data
                         (
                             handle,
                             0x03, /* TODO: Assuming single packet. Update to handle fragmentation */
                             datalen,
                             data
                         );

                if (API_SUCCESS == retval)
                {
                    /*
                     * If periodic advertising is currently enabled for the specified
                     * advertising set, the Controller shall use the new data in
                     * subsequent periodic advertising events for this advertising set.
                     * If a periodic advertising event is in progress when this
                     * command is issued, the Controller may use the old or new data
                     * for that event
                     */
                    retval = BT_hci_le_set_periodic_advertising_enable
                             (
                                 0x01,
                                 handle
                             );
                }
            }
            else
            {
                /*
                 * data cannot be NULL as it is filled by the above layer
                 * explicitly, However there is a possibilty that the call to
                 * BT_hci_le_set_periodic_advertising_parameters() or
                 * BT_hci_le_set_periodic_advertising_data() may return in a
                 * status other than GA_SUCCESS, in which case no further
                 * transaction shall occur and no event is awaited.
                 */
            }
        }
        else if (GA_BRR_ADV_TYPE_EA == type)
        {
            retval = BT_hci_le_set_extended_advertising_parameters
                     (
                         handle,
                         ga_brr_eadv_params.adv_event_properties,
                         ga_brr_eadv_params.pri_adv_interval_min,
                         ga_brr_eadv_params.pri_adv_interval_max,
                         ga_brr_eadv_params.pri_adv_channel_map,
                         ga_brr_eadv_params.own_addr_type,
                         ga_brr_eadv_params.peer_addr_type,
                         ga_brr_eadv_params.peer_addr,
                         ga_brr_eadv_params.adv_filter_policy,
                         ga_brr_eadv_params.adv_tx_power,
                         ga_brr_eadv_params.pri_adv_phy,
                         ga_brr_eadv_params.sec_adv_max_skip,
                         ga_brr_eadv_params.sec_adv_phy,
                         (handle & 0x0F), /* Derive SID from Handle for 4 bit validity */
                         ga_brr_eadv_params.scan_req_nfy_enable
                     );

            if ((GA_SUCCESS == retval) && (NULL != data))
            {
                retval = BT_hci_le_set_extended_advertising_data
                         (
                             handle,
                             0x03, /* TODO: Assuming single packet. Update to handle fragmentation */
                             ga_brr_eadv_params.frag_pref,
                             datalen,
                             data
                        );

                if (API_SUCCESS == retval)
                {
                    /*
                     * If advertising is currently enabled for the specified
                     * advertising set, the Controller shall use the new data in
                     * subsequent extended advertising events for this advertising set.
                     * If an extended advertising event is in progress when this
                     * command is issued, the Controller may use the old or new data for that
                     * event.
                     */
                    retval = BT_hci_le_set_extended_advertising_enable
                             (
                                 0x01,
                                 0x01,
                                 &handle,
                                 &ga_brr_eadv_params.duration,
                                 &ga_brr_eadv_params.max_ext_adv_evts
                             );
                }
            }
            else
            {
                /*
                 * data cannot be NULL as it is filled by the above layer
                 * explicitly, However there is a possibilty that the call to
                 * BT_hci_le_set_extended_advertising_parameters() or
                 * BT_hci_le_set_extended_advertising_data() may return in a
                 * status other than GA_SUCCESS, in which case no further
                 * transaction shall occur and no event is awaited.
                 */
            }
        }
    }

    if (GA_SUCCESS == retval)
    {
        /* Set the Expected response/Event etc */
        ga_brr_act_ctx[id].expt_event = GA_BRR_CONFIGURE_ADV_CNF;

        /* Set the Activity State */
        ga_brr_act_ctx[id].busy = BT_TRUE;
    }
    else
    {
        /* Delete any Activity created above */
        ga_brr_delete_activity_pl(pid);
    }

    /* Unlock */
    BRR_UNLOCK();

    GA_BRR_PL_TRC(
    "[GA_BRR_PL]: << ga_brr_config_adv, retval 0x%04X\n", retval);

    return retval;
}

GA_RESULT ga_brr_config_scan
          (
              GA_BRR_PRF_HANDLE   pid,
              UCHAR               enable,
              void              * context
          )
{
    GA_RESULT retval;
    UCHAR     id;
    UCHAR     create_ctx;

    retval = GA_FAILURE;
    id = GA_BRR_MAX_ACTIVITY_CNTX;
    create_ctx = BT_TRUE;

    retval = GA_FAILURE;
    GA_IGNORE_UNUSED_PARAM(context);

    GA_BRR_PL_TRC(
    "[GA_BRR_PL]: >> ga_brr_config_scan\n");

    GA_BRR_PL_TRC(
    "[GA_BRR_PL]: Searching BRR PL Activity Context ...\n");

    /* Lock */
    BRR_LOCK();

    /**
     * Search for an existing Activity for this pid with the
     * state set to "Busy".
     */
    retval = ga_brr_find_activity_by_pid_pl(pid, &id);

    if (GA_SUCCESS == retval)
    {
        if (BT_TRUE == ga_brr_act_ctx[id].busy)
        {
            GA_BRR_PL_ERR(
            "[GA_BRR_PL]: Context exists for device at 0x%02X with operation"
            " for Profile ID 0x%02X\n", id, pid);

            /* Unlock */
            BRR_UNLOCK();

            return GA_FAILURE;
        }
    }

    /* Do not reuse the context even if it exists, Update the context */
    if (BT_TRUE == create_ctx)
    {
        retval = ga_brr_add_to_activity_pl
                 (
                     pid,
                     NULL,
                     context,
                     &id
                 );

        if (GA_SUCCESS != retval)
        {
            GA_BRR_PL_ERR(
            "[GA_BRR_PL]: Failed to create activity context for Profile ID"
            " 0x%02X\n", pid);

            /* Unlock */
            BRR_UNLOCK();

            return retval;
        }
    }

    if (enable)
    {
        retval = BT_hci_le_set_extended_scan_parameters
                 (
                     ga_brr_escan_params.own_addr_type,
                     ga_brr_escan_params.scan_filter_policy,
                     ga_brr_escan_params.scan_phy,
                     &ga_brr_escan_params.scan_type,
                     &ga_brr_escan_params.scan_interval,
                     &ga_brr_escan_params.scan_window
                 );

        if (GA_SUCCESS == retval)
        {
            retval = BT_hci_le_set_extended_scan_enable
                     (
                         0x01,
                         ga_brr_escan_params.filter_duplicates,
                         ga_brr_escan_params.duration,
                         ga_brr_escan_params.period
                     );
        }
    }
    else
    {
        retval = BT_hci_le_set_extended_scan_enable
                 (
                     0x00,
                     ga_brr_escan_params.filter_duplicates,
                     ga_brr_escan_params.duration,
                     ga_brr_escan_params.period
                 );
    }

    if (GA_SUCCESS == retval)
    {
        /* Set the Expected response/Event etc */
        ga_brr_act_ctx[id].expt_event = GA_BRR_CONFIGURE_SCAN_CNF;

        /* Set the Activity State */
        ga_brr_act_ctx[id].busy = BT_TRUE;
    }
    else
    {
        /* Delete any Activity created above */
        ga_brr_delete_activity_pl(pid);
    }

    /* Unlock */
    BRR_UNLOCK();

    GA_BRR_PL_TRC(
    "[GA_BRR_PL]: << ga_brr_config_scan, retval 0x%04X\n", retval);

    return retval;
}

GA_RESULT ga_brr_config_sync
          (
              GA_BRR_PRF_HANDLE pid,
              UINT16            handle,
              GA_BRR_DEVICE   * device,
              void            * context
          )
{
    GA_RESULT retval;
    UCHAR     id;
    UCHAR     create_ctx;

    retval = GA_FAILURE;
    id = GA_BRR_MAX_ACTIVITY_CNTX;
    create_ctx = BT_TRUE;

    GA_BRR_PL_TRC(
    "[GA_BRR_PL]: >> ga_brr_config_sync\n");

    GA_BRR_PL_TRC(
    "[GA_BRR_PL]: Searching BRR PL Activity Context ...\n");

    /* Lock */
    BRR_LOCK();

    /**
     * Search for an existing Activity for this pid with the
     * state set to "Busy".
     */
    retval = ga_brr_find_activity_by_pid_pl(pid, &id);

    if (GA_SUCCESS == retval)
    {
        if (BT_TRUE == ga_brr_act_ctx[id].busy)
        {
            GA_BRR_PL_ERR(
            "[GA_BRR_PL]: Context exists for device at 0x%02X with operation"
            " for Profile ID 0x%02X\n", id, pid);

            /* Unlock */
            BRR_UNLOCK();

            return GA_FAILURE;
        }
    }

    /* Do not reuse the context even if it exists, Update the context */
    if (BT_TRUE == create_ctx)
    {
        retval = ga_brr_add_to_activity_pl
                 (
                     pid,
                     NULL,
                     context,
                     &id
                 );

        if (GA_SUCCESS != retval)
        {
            GA_BRR_PL_ERR(
            "[GA_BRR_PL]: Failed to create activity context for Profile ID"
            " 0x%02X\n", pid);

            /* Unlock */
            BRR_UNLOCK();

            return retval;
        }
    }

    if (NULL != device)
    {
        /* This is a req to Create Sync to Periodic Adv */
        retval = BT_hci_le_periodic_adv_create_sync
                 (
                     ga_brr_psync_params.options,
                     (UINT8)handle,
                     device->bd_type,
                     device->bd_addr,
                     ga_brr_psync_params.skip,
                     ga_brr_psync_params.sync_timeout,
                     ga_brr_psync_params.sync_cte_type
                 );
    }
    else if (0xFFFF != handle)
    {
        /* This is a req to terminate Periodic Adv reception */
        retval = BT_hci_le_periodic_adv_terminate_sync(handle);
    }
    else
    {
        GA_BRR_PL_ERR(
        "[GA_BRR_PL]: Invalid Params\n");

        retval = GA_BRR_INVALID_PARAMS;
    }

    if (GA_SUCCESS == retval)
    {
        /* Set the Expected response/Event etc */
        ga_brr_act_ctx[id].expt_event = GA_BRR_CONFIGURE_SYNC_CNF;

        /* Set the Activity State */
        ga_brr_act_ctx[id].busy = BT_TRUE;
    }
    else
    {
        /* Delete any Activity created above */
        ga_brr_delete_activity_pl(pid);
    }

    /* Unlock */
    BRR_UNLOCK();

    GA_BRR_PL_TRC(
    "[GA_BRR_PL]: << ga_brr_config_sync, retval 0x%04X\n", retval);

    return retval;
}

GA_RESULT ga_brr_config_broadcast
          (
              GA_BRR_PRF_HANDLE    pid,
              UINT8                handle,
              UINT8                adv_handle,
              GA_BRR_BCAST_PARAM * param,
              void               * context
          )
{
    GA_RESULT retval;
    UCHAR     id;
    UCHAR     create_ctx;

    retval = GA_FAILURE;
    id = GA_BRR_MAX_ACTIVITY_CNTX;
    create_ctx = BT_TRUE;

    GA_BRR_PL_TRC(
    "[GA_BRR_PL]: >> ga_brr_config_broadcast\n");

    GA_BRR_PL_TRC(
    "[GA_BRR_PL]: Searching BRR PL Activity Context ...\n");

    /* Lock */
    BRR_LOCK();

    /**
     * Search for an existing Activity for this pid with the
     * state set to "Busy".
     */
    retval = ga_brr_find_activity_by_pid_pl(pid, &id);

    if (GA_SUCCESS == retval)
    {
        if (BT_TRUE == ga_brr_act_ctx[id].busy)
        {
            GA_BRR_PL_ERR(
            "[GA_BRR_PL]: Context exists for device at 0x%02X with operation"
            " for Profile ID 0x%02X\n", id, pid);

            /* Unlock */
            BRR_UNLOCK();

            return GA_FAILURE;
        }
    }

    /* Do not reuse the context even if it exists, Update the context */
    if (BT_TRUE == create_ctx)
    {
        retval = ga_brr_add_to_activity_pl
                 (
                     pid,
                     NULL,
                     context,
                     &id
                 );

        if (GA_SUCCESS != retval)
        {
            GA_BRR_PL_ERR(
            "[GA_BRR_PL]: Failed to create activity context for Profile ID"
            " 0x%02X\n", pid);

            /* Unlock */
            BRR_UNLOCK();

            return retval;
        }
    }


   if (NULL != param)
    {
        /* This is a req to Create BIG specified by the bcast_params */
        retval = BT_hci_le_create_big
                 (
                     handle,
                     adv_handle,
                     param->bis_count,
                     param->sdu_interval,
                     param->max_sdu,
                     param->max_latency,
                     param->rtn,
                     param->phy,
                     param->packing,
                     param->framing,
                     param->encryption,
                     param->broadcast_code
                 );
    }
    else
    {
        /* This is a req to terminate BIG */
        retval = BT_hci_le_terminate_big(handle, 0x16U);
    }

    if (GA_SUCCESS == retval)
    {
        /* Set the Expected response/Event etc */
        ga_brr_act_ctx[id].expt_event = GA_BRR_CONFIGURE_BROADCAST_CNF;

        /* Set the Activity State */
        ga_brr_act_ctx[id].busy = BT_TRUE;
    }
    else
    {
        /* Delete any Activity created above */
        ga_brr_delete_activity_pl(pid);
    }

    /* Unlock */
    BRR_UNLOCK();

    GA_BRR_PL_TRC(
    "[GA_BRR_PL]: << ga_brr_config_broadcast, retval 0x%04X\n", retval);

    return retval;
}

GA_RESULT ga_brr_config_broadcast_sync
          (
              GA_BRR_PRF_HANDLE         pid,
              UINT8                     handle,
              UINT16                    sync_handle,
              GA_BRR_BCAST_SYNC_PARAM * sync_params,
              void                    * context
          )
{
    GA_RESULT retval;
    UCHAR     id;
    UCHAR     create_ctx;

    retval = GA_FAILURE;
    id = GA_BRR_MAX_ACTIVITY_CNTX;
    create_ctx = BT_TRUE;

    GA_BRR_PL_TRC(
    "[GA_BRR_PL]: >> ga_brr_config_broadcast_sync\n");

    GA_BRR_PL_TRC(
    "[GA_BRR_PL]: Searching BRR PL Activity Context ...\n");

    /* Lock */
    BRR_LOCK();

    /**
     * Search for an existing Activity for this pid with the
     * state set to "Busy".
     */
    retval = ga_brr_find_activity_by_pid_pl(pid, &id);

    if (GA_SUCCESS == retval)
    {
        if (BT_TRUE == ga_brr_act_ctx[id].busy)
        {
            GA_BRR_PL_ERR(
            "[GA_BRR_PL]: Context exists for device at 0x%02X with operation"
            " for Profile ID 0x%02X\n", id, pid);

            /* Unlock */
            BRR_UNLOCK();

            return GA_FAILURE;
        }
    }

    /* Do not reuse the context even if it exists, Update the context */
    if (BT_TRUE == create_ctx)
    {
        retval = ga_brr_add_to_activity_pl
                 (
                     pid,
                     NULL,
                     context,
                     &id
                 );

        if (GA_SUCCESS != retval)
        {
            GA_BRR_PL_ERR(
            "[GA_BRR_PL]: Failed to create activity context for Profile ID"
            " 0x%02X\n", pid);

            /* Unlock */
            BRR_UNLOCK();

            return retval;
        }
    }

    if (NULL != sync_params)
    {
        /* This is a req to Sync to the BIG specified by the sync_params */
        retval = BT_hci_le_big_create_sync
                 (
                     handle,
                     sync_handle,
                     sync_params->encryption,
                     sync_params->broadcast_code,
                     ga_brr_bsync_params.mse,
                     ga_brr_bsync_params.sync_timeout,
                     sync_params->bis_count,
                     sync_params->bis
                 );
    }
    else
    {
        /* This is a req to terminate Sync to BIG */
        retval = BT_hci_le_big_terminate_sync(handle);
    }

    if (GA_SUCCESS == retval)
    {
        /* Set the Expected response/Event etc */
        ga_brr_act_ctx[id].expt_event = GA_BRR_CONFIGURE_BROADCAST_SYNC_CNF;

        /* Set the Activity State */
        ga_brr_act_ctx[id].busy = BT_TRUE;
    }
    else
    {
        /* Delete any Activity created above */
        ga_brr_delete_activity_pl(pid);
    }

    /* Unlock */
    BRR_UNLOCK();

    GA_BRR_PL_TRC(
    "[GA_BRR_PL]: << ga_brr_config_broadcast_sync, retval 0x%04X\n", retval);

    return retval;
}

void ga_brr_dispatch_event_bh
     (
         void * args,
         UINT16 args_len
     )
{
    GA_BRR_DISPATCH_BH_PARAMS * params;

    GA_IGNORE_UNUSED_PARAM(args_len);

    /* Reference the args */
    params = (GA_BRR_DISPATCH_BH_PARAMS *)args;

    ga_brr_dispatch_event_handler
    (
        params->pid,
        &params->device,
        &params->event_params,
        params->context
    );
}

GA_RESULT ga_brr_dispatch_event_pl
          (
              /* IN */ GA_BRR_PRF_HANDLE        pid,
              /* IN */ GA_BRR_DEVICE           *device,
              /* IN */ GA_BRR_EVENT_PARAMS     *event_params,
              /* IN */ void                    *context
          )
{
    API_RESULT retval;
    GA_BRR_DISPATCH_BH_PARAMS params;

    /* Update the BH parameters - Assuming data in event param is NULL for now */
    params.device = *device;
    params.pid = pid;
    params.event_params = *event_params;
    params.context = context;

    /* Lock */
    BRR_LOCK();

    /* Schedule for notification to the profile callback */
    retval = write_task_enqueue
             (
                 WRITE_TASK_INTERNAL_QUEUE,
                 ga_brr_dispatch_event_bh,
                 (void *)&params,
                 sizeof(GA_BRR_DISPATCH_BH_PARAMS)
             );

    /* Unlock */
    BRR_UNLOCK();

    return retval;
}

GA_RESULT GA_set_extended_scan_parameters
          (
              GA_BRR_EXTENDED_SCAN_PARAMS * params
          )
{
    GA_RESULT retval;

    /* Initialize */
    retval = GA_FAILURE;

    if (NULL != params)
    {
        ga_brr_escan_params = *params;
        retval = GA_SUCCESS;
    }

    return retval;
}

GA_RESULT GA_set_extended_adv_parameters
          (
              GA_BRR_EXTENDED_ADV_PARAMS * params
          )
{
    GA_RESULT retval;

    /* Initialize */
    retval = GA_FAILURE;

    if (NULL != params)
    {
        ga_brr_eadv_params = *params;
        retval = GA_SUCCESS;
    }

    return retval;
}

GA_RESULT GA_set_periodic_sync_parameters
          (
              GA_BRR_PERIODIC_SYNC_PARAMS * params
          )
{
    GA_RESULT retval;

    /* Initialize */
    retval = GA_FAILURE;

    if (NULL != params)
    {
        ga_brr_psync_params = *params;
        retval = GA_SUCCESS;
    }

    return retval;
}

GA_RESULT GA_set_periodic_adv_parameters
          (
              GA_BRR_PERIODIC_ADV_PARAMS * params
          )
{
    GA_RESULT retval;

    /* Initialize */
    retval = GA_FAILURE;

    if (NULL != params)
    {
        ga_brr_padv_params = *params;
        retval = GA_SUCCESS;
    }

    return retval;
}

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
          )
{
    ATT_UUID  s_uuid;
    GA_RESULT retval;
    UCHAR     pl_id, id;
    UCHAR     create_ctx;

    retval     = GA_FAILURE;
    id         = GA_BRR_MAX_ACTIVITY_CNTX;
    pl_id      = GA_BRR_MAX_DEVICE_BRR_LIST_COUNT;
    create_ctx = BT_TRUE;

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: >> ga_brr_discover_service_pl\n");

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: Searching BRR PL Activity Context ...\n");

    /* Lock */
    BRR_LOCK();

    /**
     * Search for an existing Activity for this pid with the
     * state set to "Busy".
     */
    retval = ga_brr_find_activity_by_pid_pl(pid, &id);

    if (GA_SUCCESS == retval)
    {
        if (BT_TRUE == ga_brr_act_ctx[id].busy)
        {
            GA_BRR_PL_ERR (
            "[GA_BRR_PL]: Context exists for device at 0x%02X with operation"
            " for Profile ID 0x%02X with UUID 0x%04X and discovery Type 0x%02X\n",
            id, pid, svc_info->uuid, disc_type);

            /* Unlock */
            BRR_UNLOCK();

            return GA_FAILURE;
        }
        else
        {
            /**
             * Found a context matching the pid but its set to "Not busy",
             * Can reuse the context.
             */
            create_ctx = BT_FALSE;
        }
    }

    if(BT_TRUE == create_ctx)
    {
        retval = ga_brr_add_to_activity_pl
                 (
                     pid,
                     device,
                     context,
                     &id
                 );

        if (GA_SUCCESS != retval)
        {
            GA_BRR_PL_ERR (
            "[GA_BRR_PL]: Failed to create activity context for Profile ID"
            " ID 0x%02X with BRR DEV " GA_BRR_DEVICE_FRMT_SPECIFIER "\n",
            GA_BRR_DEVICE_PRINT_STR(device), pid);

            /* Unlock */
            BRR_UNLOCK();

            return retval;
        }
    }

    /**
     * TODO:
     * Fetch the ATT ID from the
     * - List of ATT maintained per Device.
     * Currently, BRR PL will maintain 1 ATT Handle Per
     * GA_BRR_DEVICE Address.
     * This DataStructure needs to be modified once multiple
     * ATT bearers are supported to a same device.
     */
    retval = ga_brr_search_dev_list_by_dev_pl
             (
                 device,
                 &pl_id
             );

    if (GA_SUCCESS == retval)
    {
        /* Pack the Requested UUID */
        s_uuid.uuid_16 = svc_info->uuid;

        if (GA_BRR_PS_DISCOVERY == disc_type)
        {
            /* Primary Service Discovery */
            retval = gatt_discover_ps
                     (
                         &ga_brr_dev_list_pl[pl_id].att_handle,
                         s_uuid,
                         ATT_16_BIT_UUID_FORMAT
                     );

            /* Set the Expected response/Event etc */
            ga_brr_act_ctx[id].expt_event = GATT_PS_DISCOVERY_RSP;
        }
        else if (GA_BRR_IS_DISCOVERY == disc_type)
        {
            /* Included Service Discovery */
            retval = gatt_discover_is
                     (
                         &ga_brr_dev_list_pl[pl_id].att_handle,
                         svc_info->range.start,
                         svc_info->range.end
                     );

            /* Set the Expected response/Event etc */
            ga_brr_act_ctx[id].expt_event = GATT_IS_DISCOVERY_RSP;
        }
        else if (GA_BRR_SS_DISCOVERY == disc_type)
        {
            /* Secondary Service Discovery */
            retval = gatt_discover_ss
                     (
                         &ga_brr_dev_list_pl[pl_id].att_handle,
                         s_uuid,
                         ATT_16_BIT_UUID_FORMAT
                     );

            /* Set the Expected response/Event etc */
            ga_brr_act_ctx[id].expt_event = GATT_SS_DISCOVERY_RSP;
        }

        /* TODO: Review */
        if (GA_SUCCESS == retval)
        {
            /* Set the Activity State */
            ga_brr_act_ctx[id].busy = BT_TRUE;
        }
        else
        {
            GA_BRR_PL_ERR (
            "[GA_BRR_PL]: Discover of Type 0x%02X failed, 0x%04X\n",
            disc_type, retval);
        }
    }
    else
    {
        GA_BRR_PL_ERR (
        "[GA_BRR_PL]: Search for ATT Handle by BRR Device Failed, 0x%04X\n",
        retval);
    }

    /* Handle any Failure here */
    if (GA_SUCCESS != retval)
    {
        /* Delete any Activity created above */
        ga_brr_delete_activity_pl(pid);
    }

    /* Unlock */
    BRR_UNLOCK();

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: << ga_brr_discover_service_pl, retval 0x%04X\n", retval);

    return retval;
}

GA_RESULT ga_brr_get_service_composition_pl
          (
              /* IN */ GA_BRR_PRF_HANDLE   pid,
              /* IN */ GA_BRR_DEVICE     * device,
              /* IN */ GA_BRR_SVC_INFO   * service,
              /* IN */ void              * context
          )
{
    GA_RESULT retval;
    UCHAR     pl_id, id;
    UCHAR     create_ctx;

    retval = GA_FAILURE;
    id     = GA_BRR_MAX_ACTIVITY_CNTX;
    pl_id  = GA_BRR_MAX_DEVICE_BRR_LIST_COUNT;
    create_ctx = BT_TRUE;

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: >> ga_brr_get_service_composition_pl\n");

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: Searching BRR PL Activity Context ...\n");

    /* Lock */
    BRR_LOCK();

    /**
     * Search for an existing Activity for this pid with the
     * state set to "Busy".
     */
    retval = ga_brr_find_activity_by_pid_pl(pid, &id);

    if (GA_SUCCESS == retval)
    {
#ifdef GA_BRR_HAVE_CHK_ACT_CTX_BUSY
        if (BT_TRUE == ga_brr_act_ctx[id].busy)
        {
            GA_BRR_PL_ERR (
            "[GA_BRR_PL]: Context exists for device at 0x%02X with operation"
            " for Profile ID 0x%02X with UUID 0x%04X\n", id, pid, service->uuid);

            /* Unlock */
            BRR_UNLOCK();

            return GA_FAILURE;
        }
        else
        {
            /**
             * Found a context matching the pid but its set to "Not busy",
             * Can reuse the context.
             */
            create_ctx = BT_FALSE;
        }
#endif /* GA_BRR_HAVE_CHK_ACT_CTX_BUSY */
    }

    if(BT_TRUE == create_ctx)
    {
        retval = ga_brr_add_to_activity_pl
                 (
                     pid,
                     device,
                     context,
                     &id
                 );

        if (GA_SUCCESS != retval)
        {
            GA_BRR_PL_ERR (
            "[GA_BRR_PL]: Failed to create activity context for Profile ID"
            " ID 0x%02X with BRR DEV " GA_BRR_DEVICE_FRMT_SPECIFIER "\n",
            GA_BRR_DEVICE_PRINT_STR(device), pid);

            /* Unlock */
            BRR_UNLOCK();

            return retval;
        }
    }

    /**
     * TODO:
     * Fetch the ATT ID from the
     * - List of ATT maintained per Device.
     * Currently, BRR PL will maintain 1 ATT Handle Per
     * Ga_BRR_DEVICE Address.
     * This DataStructure needs to be modified once multiple
     * ATT bearers are supported to a same device.
     */
    retval = ga_brr_search_dev_list_by_dev_pl
             (
                 device,
                 &pl_id
             );

    if (GA_SUCCESS == retval)
    {
        retval = gatt_discover_char
                 (
                     &ga_brr_dev_list_pl[pl_id].att_handle,
                     service->range.start,
                     service->range.end,
                     0x0000U,
                     0U
                 );

        if (API_SUCCESS == retval)
        {
            GA_BRR_SVC_INFO * t_svc_info;

            t_svc_info = (GA_BRR_SVC_INFO *)BT_alloc_mem(sizeof(GA_BRR_SVC_INFO));

            if (NULL != t_svc_info)
            {
                /* Set the Activity State */
                ga_brr_act_ctx[id].busy       = BT_TRUE;

                /* Set the Expected response/Event etc */
                ga_brr_act_ctx[id].expt_event = GATT_CHAR_DISCOVERY_RSP;

                GA_mem_copy(t_svc_info, service, sizeof(GA_BRR_SVC_INFO));

                /* Store the service info as meta info */
                ga_brr_act_ctx[id].meta_info = (void *) t_svc_info;
            }
        }
        else
        {
            GA_BRR_PL_ERR (
            "[GA_BRR_PL]: Characteristic Discovery Failed with 0x%04X\n", retval);
        }
    }
    else
    {
        GA_BRR_PL_ERR (
        "[GA_BRR_PL]: Search for ATT Handle by BRR Device Failed, 0x%04X\n",
        retval);
    }

    /* Handle any Failure here */
    if (GA_SUCCESS != retval)
    {
        /* Delete any Activity created above */
        ga_brr_delete_activity_pl(pid);
    }

    /* Unlock */
    BRR_UNLOCK();

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: << ga_brr_get_service_composition_pl, retval 0x%04X\n",
    retval);

    return retval;
}

GA_RESULT ga_brr_get_characteristic_composition_pl
          (
              /* IN */ GA_BRR_PRF_HANDLE   pid,
              /* IN */ GA_BRR_DEVICE     * device,
              /* IN */ GA_BRR_CHAR_RANGE * characteristic,
              /* IN */ void              * context
          )
{
    GA_RESULT retval;
    UCHAR     pl_id, id;
    UCHAR     create_ctx;

    retval = GA_FAILURE;
    id     = GA_BRR_MAX_ACTIVITY_CNTX;
    pl_id  = GA_BRR_MAX_DEVICE_BRR_LIST_COUNT;
    create_ctx = BT_TRUE;

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: >> ga_brr_get_characteristic_composition_pl\n");

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: Searching BRR PL Activity Context ...\n");

    /* Lock */
    BRR_LOCK();

    /**
     * Search for an existing Activity for this pid with the
     * state set to "Busy".
     */
    retval = ga_brr_find_activity_by_pid_pl(pid, &id);

    if (GA_SUCCESS == retval)
    {
#ifdef GA_BRR_HAVE_CHK_ACT_CTX_BUSY
        if (BT_TRUE == ga_brr_act_ctx[id].busy)
        {
            GA_BRR_PL_ERR (
            "[GA_BRR_PL]: Context exists for device at 0x%02X with operation"
            " for Profile ID 0x%02X with UUID 0x%04X\n", id, pid, service->uuid);

            /* Unlock */
            BRR_UNLOCK();

            return GA_FAILURE;
        }
        else
        {
            /**
             * Found a context matching the pid but its set to "Not busy",
             * Can reuse the context.
             */
            create_ctx = BT_FALSE;
        }
#endif /* GA_BRR_HAVE_CHK_ACT_CTX_BUSY */
    }

    if(BT_TRUE == create_ctx)
    {
        retval = ga_brr_add_to_activity_pl
                 (
                     pid,
                     device,
                     context,
                     &id
                 );

        if (GA_SUCCESS != retval)
        {
            GA_BRR_PL_ERR (
            "[GA_BRR_PL]: Failed to create activity context for Profile ID"
            " ID 0x%02X with BRR DEV " GA_BRR_DEVICE_FRMT_SPECIFIER "\n",
            GA_BRR_DEVICE_PRINT_STR(device), pid);

            /* Unlock */
            BRR_UNLOCK();

            return retval;
        }
    }

    /**
     * TODO:
     * Fetch the ATT ID from the
     * - List of ATT maintained per Device.
     * Currently, BRR PL will maintain 1 ATT Handle Per
     * Ga_BRR_DEVICE Address.
     * This DataStructure needs to be modified once multiple
     * ATT bearers are supported to a same device.
     */
    retval = ga_brr_search_dev_list_by_dev_pl
             (
                 device,
                 &pl_id
             );

    if (GA_SUCCESS == retval)
    {
        retval = gatt_discover_char_desc
                 (
                     &ga_brr_dev_list_pl[pl_id].att_handle,
                     characteristic->start,
                     characteristic->end
                 );

        if (API_SUCCESS == retval)
        {
            GA_BRR_CHAR_RANGE * t_char_range;

            t_char_range = (GA_BRR_CHAR_RANGE *)BT_alloc_mem(sizeof(GA_BRR_CHAR_RANGE));

            if (NULL != t_char_range)
            {
                /* Set the Activity State */
                ga_brr_act_ctx[id].busy       = BT_TRUE;

                /* Set the Expected response/Event etc */
                ga_brr_act_ctx[id].expt_event = GATT_CHAR_DESC_DISCOVERY_RSP;

                GA_mem_copy(t_char_range, characteristic, sizeof(GA_BRR_CHAR_RANGE));

                /* Store the service info as meta info */
                ga_brr_act_ctx[id].meta_info = (void *)t_char_range;
            }
        }
        else
        {
            GA_BRR_PL_ERR (
            "[GA_BRR_PL]: Characteristic Discovery Failed with 0x%04X\n", retval);
        }
    }
    else
    {
        GA_BRR_PL_ERR (
        "[GA_BRR_PL]: Search for ATT Handle by BRR Device Failed, 0x%04X\n",
        retval);
    }

    /* Handle any Failure here */
    if (GA_SUCCESS != retval)
    {
        /* Delete any Activity created above */
        ga_brr_delete_activity_pl(pid);
    }

    /* Unlock */
    BRR_UNLOCK();

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: << ga_brr_get_characteristic_composition_pl, retval 0x%04X\n",
    retval);

    return retval;
}

GA_RESULT ga_brr_read_characteristic_pl
          (
              /* IN */ GA_BRR_PRF_HANDLE pid,
              /* IN */ GA_BRR_DEVICE     * device,
              /* IN */ GA_BRR_CHR_HANDLE characteristic,
              /* IN */ void              * context
          )
{
    ATT_UUID   c_uuid;
    API_RESULT retval;
    UCHAR      pl_id, id;
    UCHAR     create_ctx;

    retval = GA_FAILURE;
    id     = GA_BRR_MAX_ACTIVITY_CNTX;
    pl_id  = GA_BRR_MAX_DEVICE_BRR_LIST_COUNT;
    create_ctx = BT_TRUE;

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: >> ga_brr_read_characteristic_pl\n");

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: Searching BRR PL Activity Context ...\n");

    /* Lock */
    BRR_LOCK();

    /**
     * Search for an existing Activity for this pid with the
     * state set to "Busy".
     */
    retval = ga_brr_find_activity_by_pid_pl(pid, &id);

    if (GA_SUCCESS == retval)
    {
        if (BT_TRUE == ga_brr_act_ctx[id].busy)
        {
            GA_BRR_PL_ERR (
            "[GA_BRR_PL]: Context exists for device at 0x%02X with operation"
            " for Profile ID 0x%02X Char Handle 0x%04X\n", id, pid, characteristic);

            /* Unlock */
            BRR_UNLOCK();

            return GA_FAILURE;
        }
        else
        {
            /**
             * Found a context matching the pid but its set to "Not busy",
             * Can reuse the context.
             */
            create_ctx = BT_FALSE;
        }
    }

    if(BT_TRUE == create_ctx)
    {
        retval = ga_brr_add_to_activity_pl
                 (
                     pid,
                     device,
                     context,
                     &id
                 );

        if (GA_SUCCESS != retval)
        {
            GA_BRR_PL_ERR (
            "[GA_BRR_PL]: Failed to create activity context for Profile ID"
            " ID 0x%02X with BRR DEV " GA_BRR_DEVICE_FRMT_SPECIFIER "\n",
            GA_BRR_DEVICE_PRINT_STR(device), pid);

            /* Unlock */
            BRR_UNLOCK();

            return retval;
        }
    }

    /**
     * TODO:
     * Fetch the ATT ID from the
     * - List of ATT maintained per Device.
     * Currently, BRR PL will maintain 1 ATT Handle Per
     * Ga_BRR_DEVICE Address.
     * This DataStructure needs to be modified once multiple
     * ATT bearers are supported to a same device.
     */
    retval = ga_brr_search_dev_list_by_dev_pl
             (
                 device,
                 &pl_id
             );

    if (GA_SUCCESS == retval)
    {
        c_uuid.uuid_16 = 0x0000;

        retval = gatt_char_read
                 (
                     &ga_brr_dev_list_pl[pl_id].att_handle,
                     characteristic,
                     0x0000,
                     c_uuid,
                     ATT_16_BIT_UUID_FORMAT
                 );

        if (API_SUCCESS == retval)
        {
            /* Set the Activity State */
            ga_brr_act_ctx[id].busy       = BT_TRUE;

            /* Set the Expected response/Event etc */
            ga_brr_act_ctx[id].expt_event = ATT_READ_RSP;
        }
        else
        {
            GA_BRR_PL_ERR (
            "[GA_BRR_PL]: Characteristic Read Failed with 0x%04X\n", retval);
        }
    }
    else
    {
        GA_BRR_PL_ERR (
        "[GA_BRR_PL]: Search for ATT Handle by BRR Device Failed, 0x%04X\n",
        retval);
    }

    /* Handle any Failure here */
    if (GA_SUCCESS != retval)
    {
        /* Delete any Activity created above */
        ga_brr_delete_activity_pl(pid);
    }

    /* Unlock */
    BRR_UNLOCK();

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: << ga_brr_read_characteristic_pl, retval 0x%04X\n", retval);

    return retval;
}

GA_RESULT ga_brr_write_characteristic_pl
          (
              /* IN */ GA_BRR_PRF_HANDLE pid,
              /* IN */ GA_BRR_DEVICE     * device,
              /* IN */ GA_BRR_CHR_HANDLE characteristic,
              /* IN */ GA_BRR_CHR_VAL    * value,
              /* IN */ UINT8             wwr,
              /* IN */ void              * context
          )
{
    ATT_WRITE_REQ_PARAM wr_req;
    GA_RESULT           retval;
    UCHAR               pl_id, id;
    UCHAR               create_ctx;

    retval     = GA_FAILURE;
    id         = GA_BRR_MAX_ACTIVITY_CNTX;
    pl_id      = GA_BRR_MAX_DEVICE_BRR_LIST_COUNT;
    create_ctx = BT_TRUE;

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: >> ga_brr_write_characteristic_pl\n");

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: Searching BRR PL Activity Context ...\n");

    /* Lock */
    BRR_LOCK();

    /**
     * Search for an existing Activity for this pid with the
     * state set to "Busy".
     */
    retval = ga_brr_find_activity_by_pid_pl(pid, &id);

    if (GA_SUCCESS == retval)
    {
        if (BT_TRUE == ga_brr_act_ctx[id].busy)
        {
            GA_BRR_PL_ERR (
            "[GA_BRR_PL]: Context exists for device at 0x%02X with operation"
            " for Profile ID 0x%02X Char Handle 0x%04X\n", id, pid, characteristic);

            /* Unlock */
            BRR_UNLOCK();

            return GA_FAILURE;
        }
        else
        {
            /**
             * Found a context matching the pid but its set to "Not busy",
             * Can reuse the context.
             */
            create_ctx = BT_FALSE;
        }
    }

    if(BT_TRUE == create_ctx)
    {
        retval = ga_brr_add_to_activity_pl
                 (
                     pid,
                     device,
                     context,
                     &id
                 );

        if (GA_SUCCESS != retval)
        {
            GA_BRR_PL_ERR (
            "[GA_BRR_PL]: Failed to create activity context for Profile ID"
            " ID 0x%02X with BRR DEV " GA_BRR_DEVICE_FRMT_SPECIFIER "\n",
            GA_BRR_DEVICE_PRINT_STR(device), pid);

            /* Unlock */
            BRR_UNLOCK();

            return retval;
        }
    }

    /**
     * TODO:
     * Fetch the ATT ID from the
     * - List of ATT maintained per Device.
     * Currently, BRR PL will maintain 1 ATT Handle Per
     * Ga_BRR_DEVICE Address.
     * This DataStructure needs to be modified once multiple
     * ATT bearers are supported to a same device.
     */

    retval = ga_brr_search_dev_list_by_dev_pl
             (
                 device,
                 &pl_id
             );

    if (GA_SUCCESS == retval)
    {
        wr_req.handle    = characteristic;
        wr_req.value.len = value->len;
        wr_req.value.val = value->val;

        /**
         * NOTE:
         * - The GA Framework sets wwr to "GA_TRUE" if Write Command or
         *   Write Without Response is to be used.
         * - But the Underlying Stack expects the "rsp" set to "BT_TRUE"
         *   if Write Request to be used.
         *   Translate the meaning of this parameter accordingly.
         */
        retval = gatt_char_wr
                 (
                     &ga_brr_dev_list_pl[pl_id].att_handle,
                     wr_req.handle,
                     wr_req.value.val,
                     wr_req.value.len,
                     (GA_TRUE == wwr) ? BT_FALSE : BT_TRUE
                 );

        if (API_SUCCESS == retval)
        {
            /* Set the Activity State */
            ga_brr_act_ctx[id].busy       = BT_TRUE;

            /* Set the Expected response/Event etc */
            /* Decide the Event based on WWR or WR */
            ga_brr_act_ctx[id].expt_event = (GA_TRUE == wwr) ?
                ATT_WRITE_CMD_TX_COMPLETE : ATT_WRITE_RSP;

            /**
             * This is currently used to set if the currently
             * operation is a normal Write Req.
             */
            ga_brr_act_ctx[id].process    = BT_FALSE;
        }
        else
        {
            GA_BRR_PL_ERR (
            "[GA_BRR_PL]: Characteristic Write Failed with 0x%04X\n", retval);
        }
    }
    else
    {
        GA_BRR_PL_ERR (
        "[GA_BRR_PL]: Search for ATT Handle by BRR Device Failed, 0x%04X\n",
        retval);
    }

    /* Handle any Failure here */
    if (GA_SUCCESS != retval)
    {
        /* Delete any Activity created above */
        ga_brr_delete_activity_pl(pid);
    }

    /* Unlock */
    BRR_UNLOCK();

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: << ga_brr_write_characteristic_pl, retval 0x%04X\n", retval);

    return retval;
}

GA_RESULT ga_brr_config_notification_pl
          (
              /* IN */ GA_BRR_PRF_HANDLE pid,
              /* IN */ GA_BRR_DEVICE     * device,
              /* IN */ GA_BRR_CHR_HANDLE characteristic,
              /* IN */ UINT8             state,
              /* IN */ void              * context
          )
{
    ATT_WRITE_REQ_PARAM wr_req;
    GA_RESULT           retval;
    UCHAR               pl_id, id;
    UCHAR               cccd_val[2];
    UINT16              cccd_state;
    UCHAR               create_ctx, rsp;

    retval     = GA_FAILURE;
    id         = GA_BRR_MAX_ACTIVITY_CNTX;
    pl_id      = GA_BRR_MAX_DEVICE_BRR_LIST_COUNT;
    create_ctx = BT_TRUE;

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: >> ga_brr_config_notification_pl\n");

    /* Lock */
    BRR_LOCK();

    /**
     * Search for an existing Activity for this pid with the
     * state set to "Busy".
     */
    retval = ga_brr_find_activity_by_pid_pl(pid, &id);

    if (GA_SUCCESS == retval)
    {
#ifdef GA_BRR_HAVE_CHK_ACT_CTX_BUSY
        if (BT_TRUE == ga_brr_act_ctx[id].busy)
        {
            GA_BRR_PL_ERR (
            "[GA_BRR_PL]: Context exists for device at 0x%02X with operation"
            " for Profile ID 0x%02X Char Handle 0x%04X\n", id, pid, characteristic);

            /* Unlock */
            BRR_UNLOCK();

            return GA_FAILURE;
        }
        else
        {
            /**
             * Found a context matching the pid but its set to "Not busy",
             * Can reuse the context.
             */
            create_ctx = BT_FALSE;
        }
#endif /* GA_BRR_HAVE_CHK_ACT_CTX_BUSY */
    }

    if(BT_TRUE == create_ctx)
    {
        retval = ga_brr_add_to_activity_pl
                 (
                     pid,
                     device,
                     context,
                     &id
                 );

        if (GA_SUCCESS != retval)
        {
            GA_BRR_PL_ERR (
            "[GA_BRR_PL]: Failed to create activity context for Profile ID"
            " ID 0x%02X with BRR DEV " GA_BRR_DEVICE_FRMT_SPECIFIER "\n",
            GA_BRR_DEVICE_PRINT_STR(device), pid);

            /* Unlock */
            BRR_UNLOCK();

            return retval;
        }
    }

    /**
     * TODO:
     * Fetch the ATT ID from the
     * - List of ATT maintained per Device.
     * Currently, BRR PL will maintain 1 ATT Handle Per
     * Ga_BRR_DEVICE Address.
     * This DataStructure needs to be modified once multiple
     * ATT bearers are supported to a same device.
     */

    retval = ga_brr_search_dev_list_by_dev_pl
             (
                 device,
                 &pl_id
             );

    if (GA_SUCCESS == retval)
    {
        rsp = (state & GA_BRR_NTF_RESPONSE_MASK)? GA_TRUE: GA_FALSE;
        state &= (UCHAR)(~((UCHAR)GA_BRR_NTF_RESPONSE_MASK));

        /**
         * Translating the "state" to BT Spec defined
         * values for Notification state Enabled and Disabled.
         */
        if (GA_TRUE != rsp)
        {
            cccd_state = (1 == state) ? 0x0001 : 0x0000;
        }
        else
        {
            cccd_state = (1 == state) ? 0x0002 : 0x0000;
        }

        GA_PACK_LE_2_BYTE_VAL(cccd_val, cccd_state);

        /**
         * The CCCD handle is assumed to be the handle
         * right after the Characteristic Value
         */
        wr_req.handle    = characteristic;
        wr_req.value.len = 2;
        wr_req.value.val = cccd_val;

        retval = gatt_char_wr
                 (
                     &ga_brr_dev_list_pl[pl_id].att_handle,
                     wr_req.handle,
                     wr_req.value.val,
                     wr_req.value.len,
                     0x01
                 );

        if (API_SUCCESS == retval)
        {
            /* Set the Activity State */
            ga_brr_act_ctx[id].busy       = BT_TRUE;

            /* Set the Expected response/Event etc */
            ga_brr_act_ctx[id].expt_event = ATT_WRITE_RSP;

            /**
             * This is currently used to set if the currently
             * operation is a CCCD Write.
             */
            ga_brr_act_ctx[id].process    = BT_TRUE;
        }
        else
        {
            GA_BRR_PL_ERR (
            "[GA_BRR_PL]: CCCD Configuration Failed with 0x%04X\n", retval);
        }
    }
    else
    {
        GA_BRR_PL_ERR (
        "[GA_BRR_PL]: Search for ATT Handle by BRR Device Failed, 0x%04X\n",
        retval);
    }

    /* Handle any Failure here */
    if (GA_SUCCESS != retval)
    {
        /* Delete any Activity created above */
        ga_brr_delete_activity_pl(pid);
    }

    /* Unlock */
    BRR_UNLOCK();

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: << ga_brr_config_notification_pl, retval 0x%04X\n", retval);

    return retval;
}

/* ----------------------------------------------------------------------- */
/* =========================Service - Related============================= */
/* ----------------------------------------------------------------------- */
GA_RESULT ga_brr_add_service_pl
          (
              /* IN  */ GA_BRR_UUID     service_uuid,
              /* IN  */ UINT8           service_type,
              /* IN */  UCHAR           lkid,
              /* OUT */ GA_BRR_SVC_INST * sid
          )
{
    GA_RESULT            retval;
#ifdef GATT_DB_DYNAMIC
    GATT_DB_SERVICE_INFO service_info;
    UINT16               num_attr_handles;
    UINT16               service_handle;
#else /* GATT_DB_DYNAMIC */
    ATT_UUID             uuid;
    UCHAR                frmt;
#endif /* GATT_DB_DYNAMIC */

    retval = GA_FAILURE;

    GA_IGNORE_UNUSED_PARAM(service_type);

    GA_BRR_PL_TRC(
    "[GA_BRR_PL]: >> ga_brr_add_service_pl\n");

    GA_BRR_PL_TRC(
    "[GA_BRR_PL]: >> Fetching ID for %s Service with UUID 0x%04X\n",
    (GA_BRR_PS_TYPE == service_type) ? "Primary" : "Secondary", service_uuid);

#ifdef GATT_DB_DYNAMIC
    GA_IGNORE_UNUSED_PARAM(lkid);

    service_info.is_primary = (GA_BRR_SS_TYPE != service_type) ? BT_TRUE : BT_FALSE;
    service_info.uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
    service_info.uuid.uuid.uuid_16 = service_uuid;
    service_info.link_req          = GATT_DB_SER_SUPPORT_ANY_LINK_TYPE;
    service_info.sec_req           = (GATT_DB_SER_SECURITY_MODE1 | GATT_DB_SER_SECURITY_LEVEL1 | GATT_DB_SER_ENC_KEY_SIZE_DONT_CARE);

    /* Currently, this only needs to be non-zero value */
    num_attr_handles               = 30U;

    /* Construct the Service Needed */
    retval = BT_gatt_db_add_service
             (
                 &service_info,
                 num_attr_handles,
                 &service_handle
             );

    if (API_SUCCESS != retval)
    {
        GA_BRR_PL_TRC(
        "[GA_BRR_PL]: BT_gatt_db_add_service() failed. Result: 0x%04X\n",
         retval);
    }
    else
    {
        /* Assign Service ID */
        (*sid) = (GA_BRR_SVC_INST)service_handle;
    }

#else /* GATT_DB_DYNAMIC */
    /* Here the UUID is currently only 16Bits */
    uuid.uuid_16 = service_uuid;

    /* Setting the Format as 16 Bit */
    frmt = ATT_16_BIT_UUID_FORMAT;

    retval = BT_gatt_db_get_sid_from_uuid
             (
                 &uuid,
                 frmt,
                 lkid,
                 (GA_BRR_PS_TYPE == service_type) ? BT_TRUE : BT_FALSE,
                 sid
             );
#endif /* GATT_DB_DYNAMIC */

#if 0
    /**
     * Adding/Activating the corresponding service in the SDP Database.
     */
    if (API_SUCCESS == retval)
    {
        UCHAR sdp_inst;

        /* Invalid Handle */
        sdp_inst = 0xFF;

        switch (service_uuid)
        {
            case GA_SERVICE_VCS:
                sdp_inst = DB_RECORD_VCS;
                break;
            case GA_SERVICE_VOCS:
                sdp_inst = DB_RECORD_VOCS;
                break;
            case GA_SERVICE_AICS:
                sdp_inst = DB_RECORD_AICS;
                break;
            case GA_SERVICE_MICS:
                sdp_inst = DB_RECORD_MICS;
                break;
            case GA_SERVICE_GMCS: /* Fall-through */
            case GA_SERVICE_MCS:
                sdp_inst = DB_RECORD_MCS;
                break;
            case GA_SERVICE_GTBS: /* Fall-through */
            case GA_SERVICE_TBS:
                sdp_inst = DB_RECORD_TBS;
                break;
            case GA_SERVICE_CSIS:
                sdp_inst = DB_RECORD_CSIS;
                break;
            default:
                /* Should not reach here */
                GA_BRR_PL_ERR(
                "[GA_BRR_PL]: Unknown Service UUID:0x%04x\n", service_uuid);
                break;
        }

        if (0xFF != sdp_inst)
        {
            /* Activate the SDP Record */
            appl_set_gatt_service_in_sdp_record
            (
                *sid,
                sdp_inst
            );
        }
    }
#endif /* 0 */

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: << ga_brr_add_service_pl, retval 0x%04X\n", retval);

    return retval;
}

GA_RESULT ga_brr_include_service_add_pl
          (
              /* IN  */ GA_BRR_SVC_INST sid,
              /* IN  */ GA_BRR_UUID     service_uuid,
              /* IN  */ UINT8           service_state,
              /* IN */  UCHAR           lkid,
              /* IN */ GA_BRR_SVC_INST * isid
          )
{
    GA_RESULT retval;

    retval = GA_SUCCESS;

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: >> ga_brr_include_service_add_pl\n");

#ifdef GATT_DB_DYNAMIC
    GA_IGNORE_UNUSED_PARAM(lkid);
    GA_IGNORE_UNUSED_PARAM(service_uuid);

    /* Add included service */
    retval = BT_gatt_db_add_included_service(sid, *isid);

    if (GA_SUCCESS == retval)
    {
        /* Disable service definition if requested */
        if (GA_FALSE == service_state)
        {
            /* Disable Service definition from Including Service */
            BT_gatt_db_disable_included_service(sid, *isid);
        }
    }
#else /* GATT_DB_DYNAMIC */

    GA_IGNORE_UNUSED_PARAM(sid);

    /**
     * The Last Known ID parameter is required to be GA_BRR_INST_DONTCARE
     * in all the Included Service Additions for Static GATT DB.
     * This is because, in a Static GATT DB there exists a preconstructed
     * relation established between a given sid(service identifier) and
     * its corresponding isid(included service identifier).
     *
     * In the current GA core, the Services that are Included by another
     * service are Added/Invoked prior to the parent service which includes it.
     * There by, the GA BRR layer is already aware of the Included Service UUID
     * from its UUID context and is able to provide an "LastKnowID", which is
     * not desired in this current search.
     *
     * Thus resetting the LastKnowID to GA_BRR_INST_DONTCARE here.
     */
    lkid = GA_BRR_INST_DONTCARE;

    retval = ga_brr_add_service_pl
             (
                 service_uuid,
                 GA_BRR_SS_TYPE,
                 lkid,
                 isid
             );
#endif /* GATT_DB_DYNAMIC */

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: << ga_brr_include_service_add_pl, retval 0x%04X\n", retval);

    return retval;
}

/* Wrapper to add CCCD or SCCD to a Characteristic */
DECL_STATIC API_RESULT ga_brr_add_char_ccd_pl
                       (
                           GA_BRR_SVC_INST service_handle,
                           GA_BRR_CHR_INST char_handle,
                           UCHAR           is_cccd
                       )
{
#ifdef GATT_DB_DYNAMIC
    API_RESULT        retval;
    UINT16            perm;
    ATT_VALUE         desc_value;
    GATT_DB_UUID_TYPE desc_uuid;

    /* Default value of CCCD or SCCCD */
    UCHAR ccd_value[2U]    = { 0x00U, 0x00U };

    /* Initialize Permission */
    perm                   = (GATT_DB_PERM_READ | GATT_DB_PERM_WRITE);

    /* Assign UUID as CCCD or SCCCD based on the "is_cccd" flag */
    desc_uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
    desc_uuid.uuid.uuid_16 = (BT_TRUE == is_cccd) ? GATT_CLIENT_CONFIG : GATT_SERVER_CONFIG;

    desc_value.val         = ccd_value;
    desc_value.len         = 2U;
    desc_value.actual_len  = desc_value.len;

    /* Add descriptor CCCD */
    retval = BT_gatt_db_add_characteristic_descriptor
             (
                 service_handle,
                 char_handle,
                 &desc_uuid,
                 perm,
                 &desc_value
             );

    return retval;
#else /* GATT_DB_DYNAMIC */
    return API_SUCCESS;
#endif /* GATT_DB_DYNAMIC */
}

GA_RESULT ga_brr_add_characteristic_pl
          (
              /* IN  */ GA_BRR_SVC_INST sid,
              /* IN  */ GA_BRR_UUID     char_uuid,
              /* IN  */ UINT16          char_property,
              /* IN  */ GA_BRR_CHR_VAL  * value,
              /* IN */  UCHAR           lkid,
              /* OUT */ GA_BRR_CHR_INST * cid
          )
{
    GA_RESULT         retval;
#ifdef GATT_DB_DYNAMIC
    GATT_DB_UUID_TYPE characteristic_uuid;
    ATT_VALUE         char_val;
    UINT16            char_handle;
    UINT16            perm;
#else /* GATT_DB_DYNAMIC */
    ATT_UUID          uuid;
    UCHAR             frmt;
#endif /* GATT_DB_DYNAMIC */

    retval = GA_SUCCESS;

    GA_IGNORE_UNUSED_PARAM(char_property);
    GA_IGNORE_UNUSED_PARAM(value);

    GA_BRR_PL_TRC(
    "[GA_BRR_PL]: >> ga_brr_add_characteristic_pl\n");

    GA_BRR_PL_TRC(
    "[GA_BRR_PL]: >> Added Characterisitc with UUID 0x%04X to Service ID 0x%04X "
     "requested\n", char_uuid, sid);

#ifdef GATT_DB_DYNAMIC

    GA_IGNORE_UNUSED_PARAM(lkid);

    /* Initialize */
    perm = GATT_DB_PERM_NONE;

    /**
     * NOTE:
     * Assign the UUID here.
     * Currently handling only 16Bit UUID here in GA.
     */
    characteristic_uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
    characteristic_uuid.uuid.uuid_16 = char_uuid;

    if (NULL != value)
    {
        char_val.val = value->val;
        char_val.len = value->len;
    }
    else
    {
        char_val.val = NULL;
        char_val.len = 0;
    }

    char_val.actual_len = char_val.len;

    /**
     * This needs to be relook. Currently 'perm' parameter is not yet all used
     * inside BT_gatt_db_add_characteristic api
     */
    if (GATT_DB_CHAR_READ_PROPERTY == (char_property & GATT_DB_CHAR_READ_PROPERTY))
    {
        perm |= GATT_DB_PERM_READ;
    }

    if (GATT_DB_CHAR_WRITE_PROPERTY == (char_property & GATT_DB_CHAR_WRITE_PROPERTY))
    {
        perm |= GATT_DB_PERM_WRITE;
    }

    if (GATT_DB_CHAR_WRITE_WITHOUT_RSP_PROPERTY ==
       (char_property & GATT_DB_CHAR_WRITE_WITHOUT_RSP_PROPERTY))
    {
        perm |= GATT_DB_PERM_WRITE;
    }

    retval = BT_gatt_db_add_characteristic
             (
                 (UINT16)sid,
                 &characteristic_uuid,
                 perm,
                 (UINT16)char_property,
                 &char_val,
                 &char_handle
             );

    if (API_SUCCESS != retval)
    {
        GA_BRR_PL_TRC(
        "[GA_BRR_PL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
        retval);
    }
    else
    {
        /* Assign characteristic ID */
        (*cid) = (GA_BRR_CHR_INST)char_handle;

        /* Add the CCCD or SCCD Descriptors here */
        if ((GATT_DB_CHAR_NOTIFY_PROPERTY == (char_property & GATT_DB_CHAR_NOTIFY_PROPERTY)) ||
            (GATT_DB_CHAR_INDICATE_PROPERTY == (char_property & GATT_DB_CHAR_INDICATE_PROPERTY)))
        {
            /* Add CCCD here */
            ga_brr_add_char_ccd_pl
            (
                sid,
                *cid,
                BT_TRUE
            );
        }
        else if (GATT_DB_CHAR_BROADCAST_PROPERTY == (char_property & GATT_DB_CHAR_BROADCAST_PROPERTY))
        {
            /* Add SCCD here */
            ga_brr_add_char_ccd_pl
            (
                sid,
                *cid,
                BT_FALSE
            );
        }
    }
#else /* GATT_DB_DYNAMIC */
    /* Here the UUID is currently only 16Bits */
    uuid.uuid_16 = char_uuid;

    /* Setting the Format as 16 Bit */
    frmt = ATT_16_BIT_UUID_FORMAT;

    retval = BT_gatt_db_get_cid_from_uuid
             (
                 &uuid,
                 frmt,
                 lkid,
                 sid,
                 cid
             );
#endif /* GATT_DB_DYNAMIC */

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: << ga_brr_add_characteristic_pl, retval 0x%04X\n", retval);

    return retval;
}

GA_RESULT ga_brr_manage_service_pl
          (
              /* IN  */ UINT8 sid,
              /* IN  */ UINT8 iid,
              /* IN  */ UINT8 state
          )
{
    GA_RESULT retval;

    retval = BT_gatt_db_enable_disable_service(sid, state);

    if (GA_SUCCESS == retval)
    {
        if (GA_BRR_SERVICE_ID_INVALID != iid)
        {
            /* TODO: Enable/Disable Service definition from Including Service */
            BT_gatt_db_enable_disable_included_service(iid, sid, state);
        }
    }

    return retval;
}

GA_RESULT ga_brr_manage_characteristic_pl
          (
              /* IN  */ UINT8 sid,
              /* IN  */ UINT8 cid,
              /* IN  */ UINT8 state
          )
{
    GA_IGNORE_UNUSED_PARAM(sid);

    return BT_gatt_db_enable_disable_characteristic(cid, state);
}

GA_RESULT ga_brr_update_characteristic_value_pl
          (
              /* IN */ GA_BRR_SVC_INST sid,
              /* IN */ GA_BRR_UUID     char_uuid,
              /* IN */ GA_BRR_CHR_INST cid,
              /* IN */ GA_BRR_CHR_VAL  * value
          )
{
    GA_RESULT retval;

    retval = GA_SUCCESS;

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: >> ga_brr_update_characteristic_value_pl\n");

    GA_IGNORE_UNUSED_PARAM(sid);
    GA_IGNORE_UNUSED_PARAM(char_uuid);
    GA_IGNORE_UNUSED_PARAM(cid);
    GA_IGNORE_UNUSED_PARAM(value);

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: << ga_brr_update_characteristic_value_pl, retval 0x%04X\n", retval);

    return retval;
}

GA_RESULT ga_brr_write_characteristic_rsp_pl
          (
              /* IN */ GA_BRR_DEVICE      * device,
              /* IN */ GA_BRR_CHR_CONTEXT * ctx,
              /* IN */ UINT16             status
          )
{
    GA_RESULT retval;
    UCHAR     pl_id;

    retval = GA_SUCCESS;

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: >> ga_brr_write_characteristic_rsp_pl\n");

    /* Lock */
    BRR_LOCK();

    retval = ga_brr_search_dev_list_by_dev_pl
             (
                 device,
                 &pl_id
             );

    if (GA_SUCCESS != retval)
    {
        GA_BRR_PL_ERR (
        "[GA_BRR_PL]: Search for ATT Handle by BRR Device Failed with retval"
        " 0x%04X\n", retval);

        /* Unlock */
        BRR_UNLOCK();

        return retval;
    }

    if (GA_SUCCESS == status)
    {
        retval = BT_att_write_rsp
                 (
                     &ga_brr_dev_list_pl[pl_id].att_handle
                 );

        GA_BRR_PL_TRC (
        "[GA_BRR_PL]:Write Response sent with result 0x%04X\n",retval);
    }
    else
    {
        ATT_ERROR_RSP_PARAM  err_param;
        GA_BRR_CHR_CONTEXT   * char_ctx;

        /* TODO: Check how to fetch if its a READ BLOB? */
        err_param.op_code    = ATT_WRITE_REQ;

        /**
         * Currently Sending the Status provided by Upper Layer as the
         * error code to be used in the ATT ERR Response.
         */
        err_param.error_code = (UCHAR)status;

        if (NULL != ctx)
        {
            char_ctx = (GA_BRR_CHR_CONTEXT *)ctx;

            GATT_DB_GET_CHAR_VAL_HANDLE
            (
                char_ctx->inst,
                err_param.handle
            );

            retval = BT_att_send_error_rsp
                     (
                         &ga_brr_dev_list_pl[pl_id].att_handle,
                         &err_param
                     );

            GA_BRR_PL_TRC (
            "[GA_BRR_PL]: Err Rsp for Handle:0x%04X Opcode:0x%02X Reason:"
            "0x%02X with retval 0x%04X\n", err_param.handle, err_param.op_code,
            err_param.error_code, retval);
        }
    }

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: << ga_brr_write_characteristic_rsp_pl, retval 0x%04X\n", retval);

    /* Unlock */
    BRR_UNLOCK();

    return retval;
}

GA_RESULT ga_brr_read_characteristic_rsp_pl
          (
              /* IN */ GA_BRR_DEVICE      * device,
              /* IN */ GA_BRR_CHR_CONTEXT * ctx,
              /* IN */ UINT16               status,
              /* IN */ GA_BRR_CHR_VAL     * value
          )
{
    GA_RESULT retval;
    UCHAR     pl_id;

    retval = GA_SUCCESS;

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: >> ga_brr_read_characteristic_rsp_pl\n");

    /* Lock */
    BRR_LOCK();

    retval = ga_brr_search_dev_list_by_dev_pl
             (
                 device,
                 &pl_id
             );

    if (GA_SUCCESS != retval)
    {
        GA_BRR_PL_ERR (
        "[GA_BRR_PL]: Search for ATT Handle by BRR Device Failed with retval"
        " 0x%04X\n", retval);

        /* Unlock */
        BRR_UNLOCK();

        return retval;
    }

    if (GA_SUCCESS == status)
    {
        ATT_READ_RSP_PARAM rsp;

        rsp.val = value->val;
        rsp.len = value->len;

        if ((NULL != ctx) && (GA_TRUE != ctx->is_blob))
        {
            retval = BT_att_read_rsp
                     (
                         &ga_brr_dev_list_pl[pl_id].att_handle,
                         &rsp
                     );
            GA_BRR_PL_TRC(
                "[GA_BRR_PL]:Read Response sent with result 0x%04X\n", retval);
        }
        else
        {
#ifdef ATT_READ_BLOB_SUPPORT
            retval = BT_att_read_blob_rsp
                     (
                         &ga_brr_dev_list_pl[pl_id].att_handle,
                         &rsp
                     );

            GA_BRR_PL_TRC(
                "[GA_BRR_PL]:Read Blob Response sent with result 0x%04X\n", retval);
#endif /* ATT_READ_BLOB_SUPPORT */

            /* TODO: Send error response */
        }
    }
    else
    {
        ATT_ERROR_RSP_PARAM  err_param;
        GA_BRR_CHR_CONTEXT   * char_ctx;

        err_param.error_code = (UCHAR)status;

        /* Coverity: REVERSE_INULL */
        if (NULL != ctx)
        {
            if (GA_TRUE != ctx->is_blob)
            {
                err_param.op_code = ATT_READ_REQ;
            }
            else
            {
                err_param.op_code = ATT_READ_BLOB_REQ;
            }

            char_ctx = (GA_BRR_CHR_CONTEXT *)ctx;

            GATT_DB_GET_CHAR_VAL_HANDLE
            (
                char_ctx->inst,
                err_param.handle
            );

            retval = BT_att_send_error_rsp
                     (
                         &ga_brr_dev_list_pl[pl_id].att_handle,
                         &err_param
                     );

            GA_BRR_PL_TRC (
            "[GA_BRR_PL]: Err Rsp for Handle:0x%04X Opcode:0x%02X Reason:"
            "0x%02X with retval 0x%04X\n", err_param.handle, err_param.op_code,
            err_param.error_code, retval);
        }
    }

    /* Unlock */
    BRR_UNLOCK();

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: << ga_brr_read_characteristic_rsp_pl, retval 0x%04X\n", retval);

    return retval;
}

GA_RESULT ga_brr_notify_characteristic_pl
          (
              /* IN */ GA_BRR_DEVICE   * device,
              /* IN */ GA_BRR_SVC_INST sid,
              /* IN */ GA_BRR_UUID     char_uuid,
              /* IN */ GA_BRR_CHR_INST cid,
              /* IN */ GA_BRR_CHR_VAL  * value,
              /* IN */ UINT8           nwr
          )
{
    ATT_HANDLE_VALUE_PAIR hndl_val_param;
    GA_RESULT             retval;
    UINT16                cli_cnfg;
    GATT_DB_HANDLE        db_hndl;
    UCHAR                 pl_id;
    ATT_VALUE             val;
    ATT_ATTR_HANDLE       val_hndl;
    API_RESULT            ret;

    retval   = GA_SUCCESS;
    cli_cnfg = 0;

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: >> ga_brr_notify_characteristic_pl\n");

    /* Lock */
    BRR_LOCK();

    retval = ga_brr_search_dev_list_by_dev_pl
             (
                 device,
                 &pl_id
             );

    if (GA_SUCCESS != retval)
    {
        GA_BRR_PL_ERR (
        "[GA_BRR_PL]: Search for ATT Handle by BRR Device Failed with retval"
        " 0x%04X\n", retval);

        /* Unlock */
        BRR_UNLOCK();

        return retval;
    }

    /**
     * First check if the CID instance is Valid.
     * - The UL provides valid instance of CID in cases where
     *   there are multiple instaces of a CID inside a Service.
     * - If the CID is valid, use it.
     */
    if (GA_BRR_INST_DONTCARE == cid)
    {
        ATT_UUID   uuid;
        UCHAR      frmt;

        /* Fetch the CID from SID and UUID */
        /* Here the UUID is currently only 16Bits */
        uuid.uuid_16 = char_uuid;

        /* Setting the Format as 16 Bit */
        frmt = ATT_16_BIT_UUID_FORMAT;

        ret = BT_gatt_db_get_cid_from_uuid
              (
                  &uuid,
                  frmt,
                  GA_BRR_INST_DONTCARE,
                  sid,
                  &cid
              );

        GA_BRR_PL_TRC (
        "[GA_BRR_PL]: Fetched CID 0x%02X for UUID 0x%04X inside SID 0x%02X,"
        " with retval 0x%04X\n", cid, char_uuid, sid, ret);

        if (API_SUCCESS != ret)
        {
            GA_BRR_PL_ERR (
            "[GA_BRR_PL]: Unable to find this CID for SID 0x%02X!\n",
            sid);

            /* Unlock */
            BRR_UNLOCK();

            return ret;
        }
    }

    /**
     * Check if the CCCD Corresponding to this cid is
     * Enabled or Disabled
     */
    db_hndl.device_id  = ga_brr_dev_list_pl[pl_id].att_handle.device_id;
    db_hndl.char_id    = cid;
    db_hndl.service_id = sid;

    BT_gatt_db_get_char_val_hndl (&db_hndl,&val_hndl);
    retval = BT_gatt_db_get_char_cli_cnfg(&db_hndl,&val);

    if (API_SUCCESS != retval)
    {
        cli_cnfg = 0x0000;
    }
    else
    {
        BT_UNPACK_LE_2_BYTE(&cli_cnfg, val.val);
    }

    /* Check if Fetched CCCD value is Configured for NTF */
    if (0x0000 != cli_cnfg)
    {
        hndl_val_param.handle    = val_hndl;
        hndl_val_param.value.val = value->val;
        hndl_val_param.value.len = value->len;

        GA_BRR_PL_TRC(
        "[GA_BRR_PL]: Attempting to send Notification for Handle 0x%04X"
        " with value length 0x%02X\n", val_hndl, value->len);

        GA_brr_pl_debug_dump_bytes(value->val, value->len);

        if (GA_TRUE == nwr)
        {
            retval = BT_att_send_hndl_val_ntf
                     (
                         &ga_brr_dev_list_pl[pl_id].att_handle,
                         &hndl_val_param
                     );
        }
        else
        {
            retval = BT_att_send_hndl_val_ind
                     (
                         &ga_brr_dev_list_pl[pl_id].att_handle,
                         &hndl_val_param
                     );
        }

        if (API_SUCCESS != retval)
        {
            GA_BRR_PL_ERR (
            "[GA_BRR_PL]: Failed to send Notification for Handle 0x%04X, "
            "reason 0x%04X\n", val_hndl, retval);
        }
    }
    else
    {
        GA_BRR_PL_ERR (
        "[GA_BRR_PL]: Failed to send Notification for Handle 0x%04X, "
        "as associated CCCD improperly configured:0x%04X, retval 0x%04X\n",
        val_hndl, retval, cli_cnfg);

        retval = GA_FAILURE;
    }

    /* Unlock */
    BRR_UNLOCK();

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: << ga_brr_notify_characteristic_pl, retval 0x%04X\n", retval);

    return retval;
}

/* ----------------------------------------------------------------------- */
/* ================General and Profile Role Callbacks===================== */
/* ----------------------------------------------------------------------- */

GA_RESULT ga_hci_event_callback(UINT8 event_code, UINT8* event_data, UINT8 event_datalen)
{
    GA_RESULT retval;
    UINT8 status;
    UINT8 value_1;
    UINT16 value_2;
    UINT16 ga_event_result, ga_event_datalen;
    UCHAR  id, ga_event, del_act;
    UINT8 * ga_event_data;
    UINT16  index, marker;
    GA_BRR_PRF_HANDLE pid;
    void* ctx;

    /* Initialize */
    retval = API_SUCCESS;
    ga_event_datalen = 0;
    ga_event_result = GA_SUCCESS;
    id = GA_BRR_MAX_ACTIVITY_CNTX;
    del_act = BT_FALSE;
    pid = GA_BRR_PROFILE_ID_INVALID;
    ctx = NULL;

    /* Lock */
    BRR_LOCK();

    retval = ga_brr_find_activity_pl
             (
                 &id
             );

    if (GA_SUCCESS == retval)
    {
        pid = ga_brr_act_ctx[id].pid;
        ctx = ga_brr_act_ctx[id].context;
    }

    /* By default, Set data and len as 0 */
    ga_event_datalen = 0;
    ga_event_data = NULL;

    /* By default, set the Event to Invalid event */
    ga_event = GA_BRR_EVENT_INVALID;

    /* Switch on the Event Code */
    switch (event_code)
    {
    case HCI_COMMAND_STATUS_EVENT:
        /* Status */
        hci_unpack_1_byte_param(&status, event_data);
        event_data += 1;

        /* Number of Command Packets */
        hci_unpack_1_byte_param(&value_1, event_data);
        event_data += 1;

        /* Command Opcode */
        hci_unpack_2_byte_param(&value_2, event_data);
        event_data += 2;

        /*
         * If the expected Opcode Status Event is received, Go through further
         * processing of the event to determine if further events are expected.
         */

        switch (value_2)
        {
        case HCI_LE_CREATE_CIS_OPCODE:
            break;

        case HCI_LE_PERIODIC_ADVERTISING_CREATE_SYNC_OPCODE:
            if (GA_BRR_CONFIGURE_SYNC_CNF == ga_brr_act_ctx[id].expt_event)
            {
                if (0x0 != status)
                {
                    /* Trigger to the above layer with a Failure status */
                    del_act = GA_TRUE;
                    ga_event_result = GA_FAILURE;
                    ga_event = ga_brr_act_ctx[id].expt_event;
                }
                else
                {
                    /*
                     * Status is success, Wait for the next event as per Spec
                     * HCI_LE_PERIODIC_ADVERTISING_SYNC_ESTABLISHED_SUBEVENT or
                     * HCI_LE_PERIODIC_ADVERTISING_TERMINATE_SYNC_OPCODE
                     */
                }
            }
            break;

        case HCI_LE_CREATE_BIG_OPCODE: /* fall-through */
        case HCI_LE_TERMINATE_BIG_OPCODE:
            if (GA_BRR_CONFIGURE_BROADCAST_CNF == ga_brr_act_ctx[id].expt_event)
            {
                if (0x0 != status)
                {
                    /* Trigger to the above layer with a Failure status */
                    del_act = GA_TRUE;
                    ga_event_result = GA_FAILURE;
                    ga_event = ga_brr_act_ctx[id].expt_event;
                }
                else
                {
                    /*
                     * Status is success, Wait for the next event as per Spec
                     * HCI_LE_CREATE_BIG_COMPLETE_SUBEVENT or
                     * HCI_LE_TERMINATE_BIG_COMPLETE_SUBEVENT
                     */
                }
            }
            break;

        case HCI_LE_BIG_CREATE_SYNC_OPCODE:
            if (GA_BRR_CONFIGURE_BROADCAST_SYNC_CNF == ga_brr_act_ctx[id].expt_event)
            {
                if (0x0 != status)
                {
                    /* Trigger to the above layer with a Failure status */
                    del_act = GA_TRUE;
                    ga_event_result = GA_FAILURE;
                    ga_event = ga_brr_act_ctx[id].expt_event;
                }
                else
                {
                    /*
                     * Status is success, Wait for the next event as per Spec
                     * HCI_LE_BIG_SYNC_ESTABLISHED_SUBEVENT
                     */
                }
            }
            break;

        default:
            /* MISRA C-2012 Rule 16.4 */
            break;
        }
        break;

    case HCI_COMMAND_COMPLETE_EVENT:
        /* Number of Command Packets */
        hci_unpack_1_byte_param(&value_1, event_data);
        event_data += 1;

        /* Command Opcode */
        hci_unpack_2_byte_param(&value_2, event_data);
        event_data += 2;

        /* Command Status */
        hci_unpack_1_byte_param(&status, event_data);
        event_data += 1;

        /* Command Return Parameters */
        if (event_datalen >= 4)
        {
            switch (value_2)
            {
            case HCI_LE_SET_HOST_FEATURE_OPCODE:
                break;

            case HCI_LE_SET_CIG_PARAMETERS_OPCODE:
                break;

            case HCI_LE_SETUP_ISO_DATA_PATH_OPCODE:
                break;

            case HCI_LE_REMOVE_ISO_DATA_PATH_OPCODE:
                break;

            case HCI_LE_SET_PERIODIC_ADV_PARAMS_OPCODE:
                if (GA_BRR_CONFIGURE_ADV_CNF == ga_brr_act_ctx[id].expt_event)
                {
                    /* Trigger to the above layer with failure status */
                    if (0x0 != status)
                    {
                        del_act = GA_TRUE;
                        ga_event_result = GA_FAILURE;
                        ga_event = ga_brr_act_ctx[id].expt_event;
                    }
                    else
                    {
                        /*
                         * Status is success.
                         * Post this, a call to BT_hci_le_set_periodic_advertising_data()
                         * would have been added in the chain, hence event
                         * HCI_LE_SET_PERIODIC_ADVERTISING_DATA_OPCODE is expected.
                         */
                    }
                }
                break;

            case HCI_LE_SET_PERIODIC_ADVERTISING_DATA_OPCODE:
                if (GA_BRR_CONFIGURE_ADV_CNF == ga_brr_act_ctx[id].expt_event)
                {
                    /* Trigger to the above layer with failure status */
                    if (0x0 != status)
                    {
                        del_act = GA_TRUE;
                        ga_event_result = GA_FAILURE;
                        ga_event = ga_brr_act_ctx[id].expt_event;
                    }
                    else
                    {
                        /*
                         * Status is success.
                         * Post this, a call to BT_hci_le_set_periodic_advertising_enable()
                         * would have been added in the chain, hence event
                         * HCI_LE_SET_PERIODIC_ADVERTISING_ENABLE_OPCODE is expected.
                         */

                        /* TODO: If PA is already enabled, and data is updated generate event here */
                    }
                }
                break;

            case HCI_LE_SET_PERIODIC_ADVERTISING_ENABLE_OPCODE:
                if (GA_BRR_CONFIGURE_ADV_CNF == ga_brr_act_ctx[id].expt_event)
                {
                    del_act = GA_TRUE;
                    ga_event = ga_brr_act_ctx[id].expt_event;

                    if (0x0 != status)
                    {
                        /* Trigger to the above layer with failure status */
                        ga_event_result = GA_FAILURE;
                    }
                    else
                    {
                        /* Status is success, send above */
                    }
                }
                break;

            case HCI_LE_SET_EXTENDED_ADV_PARAMS_OPCODE:
                if (GA_BRR_CONFIGURE_ADV_CNF == ga_brr_act_ctx[id].expt_event)
                {
                    /* Trigger to the above layer with failure status */
                    if (0x0 != status)
                    {
                        del_act = GA_TRUE;
                        ga_event_result = GA_FAILURE;
                        ga_event = ga_brr_act_ctx[id].expt_event;
                    }
                    else
                    {
                        /*
                         * Status is success.
                         * Post this, a call to BT_hci_le_set_extended_advertising_data()
                         * would have been added in the chain, hence event
                         * HCI_LE_SET_EXTENDED_ADVERTISING_DATA_OPCODE is expected.
                         */
                    }
                }
                break;

            case HCI_LE_SET_EXTENDED_ADVERTISING_DATA_OPCODE:
                if (GA_BRR_CONFIGURE_ADV_CNF == ga_brr_act_ctx[id].expt_event)
                {
                    /* Trigger to the above layer with failure status */
                    if (0x0 != status)
                    {
                        del_act = GA_TRUE;
                        ga_event_result = GA_FAILURE;
                        ga_event = ga_brr_act_ctx[id].expt_event;
                    }
                    else
                    {
                        /*
                         * Status is success.
                         * Post this, a call to BT_hci_le_set_extended_advertising_enable()
                         * would have been added in the chain, hence event
                         * HCI_LE_SET_EXTENDED_ADVERTISE_ENABLE_OPCODE is expected.
                         */
                    }
                }
                break;

            case HCI_LE_SET_EXTENDED_ADVERTISE_ENABLE_OPCODE:
                if (GA_BRR_CONFIGURE_ADV_CNF == ga_brr_act_ctx[id].expt_event)
                {
                    del_act = GA_TRUE;
                    ga_event = ga_brr_act_ctx[id].expt_event;

                    if (0x0 != status)
                    {
                        /* Trigger to the above layer with failure status */
                        ga_event_result = GA_FAILURE;
                    }
                    else
                    {
                        /* Status is success, send above */
                    }
                }
                break;

            case HCI_LE_SET_EXTENDED_SCAN_PARAMETERS_OPCODE:
                if (GA_BRR_CONFIGURE_SCAN_CNF == ga_brr_act_ctx[id].expt_event)
                {
                    /* Trigger to the above layer with failure status */
                    if (0x0 != status)
                    {
                        del_act = GA_TRUE;
                        ga_event_result = GA_FAILURE;
                        ga_event = ga_brr_act_ctx[id].expt_event;
                    }
                    else
                    {
                        /*
                         * Status is success.
                         * Post this, a call to BT_hci_le_set_extended_scan_enable()
                         * would have been added in the chain, hence event
                         * HCI_LE_SET_EXTENDED_SCAN_ENABLE_OPCODE is expected.
                         */
                    }
                }
                break;

            case HCI_LE_SET_EXTENDED_SCAN_ENABLE_OPCODE:
                if (GA_BRR_CONFIGURE_SCAN_CNF == ga_brr_act_ctx[id].expt_event)
                {
                    ga_event = ga_brr_act_ctx[id].expt_event;
                    del_act = GA_TRUE;

                    /* Trigger to the above layer with status */
                    if (0x0 != status)
                    {
                        ga_event_result = GA_FAILURE;
                    }
                    else
                    {
                        /* Status is success, send above */
                    }
                }
                break;

            case HCI_LE_PERIODIC_ADVERTISING_TERMINATE_SYNC_OPCODE:
                if (GA_BRR_CONFIGURE_SYNC_CNF == ga_brr_act_ctx[id].expt_event)
                {
                    del_act = GA_TRUE;
                    ga_event = ga_brr_act_ctx[id].expt_event;

                    /* Trigger to the above layer with status */
                    if (0x0 != status)
                    {
                        ga_event_result = GA_FAILURE;
                    }
                }
                break;

            case HCI_LE_BIG_TERMINATE_SYNC_OPCODE:
                if (GA_BRR_CONFIGURE_BROADCAST_SYNC_CNF == ga_brr_act_ctx[id].expt_event)
                {
                    del_act = GA_TRUE;
                    ga_event = ga_brr_act_ctx[id].expt_event;

                    /* Skip Number of Command Packets, Opcode, Status */
                    ga_event_data = event_data;
                    ga_event_datalen = (event_datalen - 3);

                    /* Trigger to the above layer with status */
                    if (0x0 != status)
                    {
                        ga_event_result = GA_FAILURE;
                    }
                }
                break;

            default:
                /* MISRA C-2012 Rule 16.4 */
                 break;
            }
        }
        break;

    case HCI_LE_META_EVENT:

        /* Extract the subevent code */
        hci_unpack_1_byte_param(&value_1, event_data);
        event_data ++;
        event_datalen --;

        /*
         * Deliver the HCI event parameters for all meta events skipping the
         * subevent code
         */
        ga_event_data = event_data;
        ga_event_datalen = event_datalen;

        switch (value_1)
        {
        case HCI_LE_ENHANCED_CONNECTION_COMPLETE_SUBEVENT:
             break;
        case HCI_LE_CIS_REQUEST_SUBEVENT:
            break;
        case HCI_LE_CIS_ESTABLISHED_SUBEVENT:
            break;
        case HCI_LE_EXTENDED_ADVERTISING_REPORT_SUBEVENT:
            /* Get the bearer ID for BAP Module */
            if (GA_SUCCESS != ga_brr_get_module_pid(GA_MODULE_ID_BAP, &pid))
            {
                break;
            }

            ga_event = GA_BRR_EXTENDED_ADV_REPORT_IND;

            /* Extract the number of reports  */
            hci_unpack_1_byte_param(&value_1, event_data);
            event_data++;
            event_datalen--;

            marker = 0;

            /* Loop through all reports and deliver */
            for (index = 0; index < value_1; index++)
            {
                if (marker > event_datalen)
                {
                    printf("*** Adv report incorrectly parsed ***\n");
                    break;
                }

                ga_event_data = (event_data + marker);
                marker += (24U + event_data[23U]);
                ga_event_datalen = (24U + event_data[23U]);

                /* Unlock */
                BRR_UNLOCK();

                if (NULL != ga_brr_pl_cb)
                {
                    /* Notify Upper Layer only if the activity is complete */
                    ga_brr_pl_cb
                    (
                        NULL,
                        NULL,
                        GA_BRR_PROFILE_ROLE,
                        &pid,
                        ga_event,
                        ga_event_result,
                        ga_event_data,
                        ga_event_datalen
                    );
                }

                /* Lock */
                BRR_LOCK();
            }

            /* Reset the event. Do not give from below again */
            ga_event = GA_BRR_EVENT_INVALID;
            break;
        case HCI_LE_PERIODIC_ADVERTISING_SYNC_ESTABLISHED_SUBEVENT:
            if (GA_BRR_CONFIGURE_SYNC_CNF == ga_brr_act_ctx[id].expt_event)
            {
                hci_unpack_1_byte_param(&status, event_data);

                del_act = GA_TRUE;
                ga_event = ga_brr_act_ctx[id].expt_event;

                if (0x0 != status)
                {
                    /* Trigger to the above layer with a Failure status */
                    ga_event_result = GA_FAILURE;
                }
            }
            break;
        case HCI_LE_PERIODIC_ADVERTISING_REPORT_SUBEVENT:
            /* Get the bearer ID for BAP Module */
            if (GA_SUCCESS != ga_brr_get_module_pid(GA_MODULE_ID_BAP, &pid))
            {
                break;
            }
            ga_event = GA_BRR_PERIODIC_ADV_REPORT_IND;
            break;
        case HCI_LE_PERIODIC_ADVERTISING_SYNC_LOST_SUBEVENT:
            /* Get the bearer ID for BAP Module */
            if (GA_SUCCESS != ga_brr_get_module_pid(GA_MODULE_ID_BAP, &pid))
            {
                break;
            }
            ga_event = GA_BRR_PERIODIC_ADV_SYNC_LOST_IND;
            break;
        case HCI_LE_SCAN_TIMEOUT_SUBEVENT:
            /* Get the bearer ID for BAP Module */
            if (GA_SUCCESS != ga_brr_get_module_pid(GA_MODULE_ID_BAP, &pid))
            {
                break;
            }
            ga_event = GA_BRR_CONFIGURE_SCAN_CNF;
            break;
        case HCI_LE_ADVERTISING_SET_TERMINATED_SUBEVENT:
            break;
        case HCI_LE_SCAN_REQUEST_RECEIVED_SUBEVENT:
            break;
        case HCI_LE_CREATE_BIG_COMPLETE_SUBEVENT:
            if (GA_BRR_CONFIGURE_BROADCAST_CNF == ga_brr_act_ctx[id].expt_event)
            {
                hci_unpack_1_byte_param(&status, event_data);

                del_act = GA_TRUE;
                ga_event = ga_brr_act_ctx[id].expt_event;

                if (0x0 != status)
                {
                    /* Trigger to the above layer with a Failure status */
                    ga_event_result = GA_FAILURE;
                }
            }
            break;
        case HCI_LE_TERMINATE_BIG_COMPLETE_SUBEVENT:
            if (GA_BRR_CONFIGURE_BROADCAST_CNF == ga_brr_act_ctx[id].expt_event)
            {
                del_act = GA_TRUE;
                ga_event = ga_brr_act_ctx[id].expt_event;
            }
            else
            {
                /* Get the bearer ID for BAP Module */
                if (GA_SUCCESS != ga_brr_get_module_pid(GA_MODULE_ID_BAP, &pid))
                {
                    break;
                }
                ga_event = GA_BRR_BIG_TERMINATE_IND;
            }
            break;
        case HCI_LE_BIG_SYNC_ESTABLISHED_SUBEVENT:
            if (GA_BRR_CONFIGURE_BROADCAST_SYNC_CNF == ga_brr_act_ctx[id].expt_event)
            {
                /* Extract the Status */
                hci_unpack_1_byte_param(&status, event_data);

                del_act = GA_TRUE;
                ga_event = ga_brr_act_ctx[id].expt_event;

                if (0x0 != status)
                {
                    /* Trigger to the above layer with a Failure status */
                    ga_event_result = GA_FAILURE;
                }
            }
            break;
        case HCI_LE_BIG_SYNC_LOST_SUBEVENT:
            /* Get the bearer ID for BAP Module */
            if (GA_SUCCESS != ga_brr_get_module_pid(GA_MODULE_ID_BAP, &pid))
            {
                break;
            }
            ga_event = GA_BRR_BROADCAST_SYNC_LOST_IND;
            break;

        case HCI_LE_BIGINFO_ADVERTISING_REPORT_SUBEVENT:
            /* Get the bearer ID for BAP Module */
            if (GA_SUCCESS != ga_brr_get_module_pid(GA_MODULE_ID_BAP, &pid))
            {
                break;
            }
            ga_event = GA_BRR_BIGINFO_ADV_REPORT_IND;
            break;

        default:
            /* MISRA C-2012 Rule 16.4 */
            break;
        }
        break;
    default:
        /* MISRA C-2012 Rule 16.4 */
        break;
    }

    if ((GA_BRR_EVENT_INVALID != ga_event) && (NULL != ga_brr_pl_cb))
    {
        if (id != GA_BRR_MAX_ACTIVITY_CNTX)
        {
            if (BT_TRUE == del_act)
            {
                /* This will allow triggering of chaining of API calls */
                ga_brr_act_ctx[id].busy = GA_FALSE;

                ga_brr_delete_activity_pl(ga_brr_act_ctx[id].pid);
            }
        }

        /* Unlock */
        BRR_UNLOCK();

        /* Notify Upper Layer only if the activity is complete */
        ga_brr_pl_cb
        (
            NULL,
            ctx,
            GA_BRR_PROFILE_ROLE, /* TODO: What in case of a Service */
            &pid,
            ga_event,
            ga_event_result,
            ga_event_data,
            ga_event_datalen
        );

        /* Lock */
        BRR_LOCK();
    }

    /* Unlock */
    BRR_UNLOCK();

    return GA_SUCCESS;
}

API_RESULT ga_brr_initiate_att_mtu_update
           (
               ATT_HANDLE     * handle,
               BT_DEVICE_ADDR * bd_addr
           )
{
    API_RESULT      retval;
    UCHAR           role;

    retval = API_SUCCESS;
    role   = 0xFF;

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: >> ga_brr_initiate_att_mtu_update\n");

    /* Find out the Role Associated with the Device ID */
    retval = BT_hci_le_get_device_role
             (
                 bd_addr,
                 &role
             );

    if (API_SUCCESS != retval)
    {
        GA_BRR_PL_ERR (
        "[GA_BRR_PL]: Unable to find Role for DEV ID 0x%02X, retval 0x%04X\n",
        handle->device_id, retval);
    }
    else
    {
#if 0
        if (0 != role)
        {
            GA_BRR_PL_ERR(
                "[GA_BRR_PL]: Slave Role for DEV ID 0x%02X, retval 0x%04X\n",
                handle->device_id, retval);
        }
        else
#endif /* 0 */
        {
            /* Initiate this for Master */
            retval = gatt_xchg_mtu
                     (
                         handle,
                         GA_BRR_DEVICE_DEFAULT_MTU,
                         0
                     );
        }
    }

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: << ga_brr_initiate_att_mtu_update, retval 0x%04X\n", retval);

    return retval;
}

/** Callback Interface from the Underlying Stack */
/**
 * TODO:
 * 1. Use an Registration API from the Underlying Stack Layer.
 * 2. Or, Have the Underlying Stack to expose one such interface.
 */
GA_RESULT ga_gatt_cb_pl
          (
              ATT_HANDLE * handle,
              UCHAR      att_event,
              API_RESULT event_result,
              void       * eventdata,
              UINT16     event_datalen
          )
{
    GA_BRR_SVC_INFO            ga_srv_info;
    GA_BRR_CHR_INFO            ga_char_info;
    GA_BRR_CHRDESC_INFO        ga_desc_info;
    GATT_SERVICE_PARAM         * service;
    GATT_CHARACTERISTIC_PARAM  * characteristic;
    GATT_CHAR_DESC_PARAM       * desc;
    GA_BRR_SERVICE_COMPOSITION ga_serv_comp;
    GA_BRR_CHARACTERISTIC_COMPOSITION ga_char_comp;

    void                      * ga_event_data;
    void                      * ul_context;
    BT_DEVICE_ADDR            bd_addr;
    GA_BRR_DEVICE             ga_dev;
    API_RESULT                retval;
    UINT16                    ga_event_result, ga_event_datalen;
    UCHAR                     pl_id, id, ga_event, role, del_act;
    GA_BRR_PRF_HANDLE         inst_id;

    /* Coverity: RETURN_LOCAL */
    GA_BRR_CHR_HANDLE         char_hndl;

    GA_IGNORE_UNUSED_PARAM(event_datalen);

    retval           = API_SUCCESS;
    ga_event_data    = NULL;
    ul_context       = NULL;
    ga_event_datalen = 0;
    ga_event_result  = GA_SUCCESS;
    ga_event         = GA_BRR_EVENT_INVALID;
    pl_id            = GA_BRR_MAX_DEVICE_BRR_LIST_COUNT;
    inst_id          = GA_BRR_PROFILE_ID_INVALID;
    role             = GA_BRR_PROFILE_ROLE;
    id               = GA_BRR_MAX_ACTIVITY_CNTX;
    del_act          = BT_FALSE;
    char_hndl        = 0;

    /* Lock */
    BRR_LOCK();

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: >> ga_gatt_cb_pl for DevID 0x%02X, ATTID 0x%02X, event %02X\n",
    handle->device_id, handle->att_id, att_event);

    /** Fetch BD Address */
    retval = ga_get_bd_addr_from_dev_hndl(&handle->device_id, &bd_addr);
    if(GA_SUCCESS != retval)
    {
        GA_BRR_PL_ERR (
        "[GA_BRR_PL]: Failed to Find BD Address for Device ID 0x%02X,"
        "retval 0x%04X\n", handle->device_id, retval);
    }
    else
    {
        GA_BRR_PL_TRC(
        "[GA_BRR_PL] Peer Address :- " BT_DEVICE_ADDR_FRMT_SPECIFIER "\n",
        BT_DEVICE_ADDR_PRINT_STR(&bd_addr));
    }

    /*
     * Apart from ATT Connection Ind
     * Search for the ATT instance and find the GA Device.
     * If GA Device and Handle Mapping not found,
     * Drop it!
     */
    if (ATT_CONNECTION_IND != att_event)
    {
        retval = ga_brr_search_dev_list_by_handle_pl
                 (
                     *handle,
                     &pl_id
                 );

        if (GA_SUCCESS != retval)
        {
            GA_BRR_PL_ERR (
            "[GA_BRR_PL]: Failed to Find GA Device associated with ATT Handle"
            "0x%02X:0x%02X,retval 0x%04X\n",
            handle->device_id, handle->att_id, retval);

            /* Unlock */
            BRR_UNLOCK();

            /* Dropping the packet */
            return API_FAILURE;
        }

        /**
         * Search the Activity Context for all Non-Server Initiated ATT Events,
         * and find if this is a expected response etc.
         * The Bearer will not be able to associate a Context and identify
         * the Profile ID for a Server Initiated ATT Operation such as
         * - Handle Value Notification
         * - Handle Value Multiple Notification
         * - Handle Value Indication
         */
        if ((ATT_HANDLE_VALUE_NTF != att_event) && (ATT_HANDLE_VALUE_IND != att_event))
        {
            retval = ga_brr_find_activity_by_dev_pl
                     (
                         &ga_brr_dev_list_pl[pl_id].brr_dev,
                         &id
                     );

            if (GA_SUCCESS != retval)
            {
                GA_BRR_PL_ERR (
                "[GA_BRR_PL]: Failed to Find Activity Context for GA Device "
                "associated with with BRR DEV " GA_BRR_DEVICE_FRMT_SPECIFIER "\n",
                GA_BRR_DEVICE_PRINT_STR(&ga_brr_dev_list_pl[pl_id].brr_dev));

                /* Unlock */
                BRR_UNLOCK();

                /* Dropping the packet */
                return API_FAILURE;
            }

            /* Store the Upper Layer Provided Context here */
            ul_context = ga_brr_act_ctx[id].context;

            /* Store the profile instance here */
            inst_id = ga_brr_act_ctx[id].pid;
        }
    }

    /*
     * In case where the rsp is not rx within timeout, send Failure error
     * code.
     */
    if (GA_SUCCESS != event_result)
    {
        ga_event_result = GA_FAILURE;
    }

    switch (att_event)
    {
    case ATT_CONNECTION_IND:
        /*
         * Search for in the Device List to find if ATT Handle is present *
         */
        /* check for failure!? */
        ga_get_ga_dev_from_bd_addr
        (
            (void *)&bd_addr,
            (void *)&handle,
            &ga_dev
        );

        retval = ga_brr_add_to_dev_list_pl
                 (
                     &ga_dev,
                     *handle,
                     &pl_id
                 );

        if (GA_SUCCESS != retval)
        {
            GA_BRR_PL_ERR (
            "[GA_BRR_PL]: Failed ga_brr_add_to_dev_list_pl, retval:0x%04x\n", retval);

            /* Unlock */
            BRR_UNLOCK();

            /* Dropping the packet */
            return API_FAILURE;
        }

        role             = GA_BRR_GENERIC_ROLE;
        ga_event         = GA_BRR_TRANSPORT_UP_IND;
        ga_event_data    = eventdata;
        ga_event_datalen = event_datalen;

        inst_id          = GA_BRR_PROFILE_ID_INVALID;

        /* Request for MTU Update */
        ga_brr_initiate_att_mtu_update
        (
            handle,
            &bd_addr
        );

        break;
    case ATT_DISCONNECTION_IND:
        /* TODO:
         * Free the Assigned Context for the BRR Device for Profile and
         * Service
         */
        /* Event To be Handled */
        role = GA_BRR_GENERIC_ROLE;
        ga_event         = GA_BRR_TRANSPORT_DOWN_IND;
        ga_event_data    = eventdata;
        ga_event_datalen = event_datalen;

        inst_id          = GA_BRR_PROFILE_ID_INVALID;
        break;

    case GATT_PS_DISCOVERY_RSP:
    case GATT_SS_DISCOVERY_RSP:
        /* Event */
        ga_event = GA_BRR_DISCOVER_SERVICE_CNF;

        /* Event Data */
        if (API_SUCCESS == event_result)
        {
            if (NULL != eventdata)
            {
                service = (GATT_SERVICE_PARAM *)eventdata;

                ga_srv_info.uuid = service->uuid.uuid_16;
                ga_srv_info.range.start = service->range.start_handle;
                ga_srv_info.range.end = service->range.end_handle;

                ga_event_result = GA_CONTINUE;

                ga_event_data = &ga_srv_info;
                ga_event_datalen = sizeof(ga_srv_info);
            }
        }

        if (GA_CONTINUE != ga_event_result)
        {
            ga_event_result = event_result;

            /* Mark for activity Deletion */
            del_act         = BT_TRUE;
        }
        break;

    case GATT_IS_DISCOVERY_RSP:
        /* Event */
        ga_event = GA_BRR_DISCOVER_SERVICE_CNF;

        /* Event Data */
        if (API_SUCCESS == event_result)
        {
            if (NULL != eventdata)
            {
                GATT_INC_SERVICE_PARAM * tmp_inc_param;

                /* Temporary Inlcuded Service Parameter */
                tmp_inc_param = (GATT_INC_SERVICE_PARAM *)eventdata;

                /* Fetching the relevant info for Included Service */
                /**
                 * NOTE:
                 * Currently providing only the Service Information
                 * to the upper layers. The location of Secondary Service
                 * Declaration ( i.e. Attribute Handle) is ignored.
                 */
                service = &tmp_inc_param->service;

                ga_srv_info.uuid = service->uuid.uuid_16;
                ga_srv_info.range.start = service->range.start_handle;
                ga_srv_info.range.end = service->range.end_handle;

                ga_event_result = GA_CONTINUE;

                ga_event_data = &ga_srv_info;
                ga_event_datalen = sizeof(ga_srv_info);
            }
        }

        if (GA_CONTINUE != ga_event_result)
        {
            ga_event_result = event_result;

            /* Mark for activity Deletion */
            del_act         = BT_TRUE;
        }
        break;

    case GATT_CHAR_DISCOVERY_RSP:

        /* Event */
        ga_event = GA_BRR_GET_SERVICE_COMPOSITION_CNF;

        if (GA_BRR_MAX_ACTIVITY_CNTX != id)
        {
            /* Check if the Activity context Instance is waiting for this
             * particular event.
             */
            if (GATT_CHAR_DISCOVERY_RSP == ga_brr_act_ctx[id].expt_event)
            {
                /* Event Data */
                if (GA_SUCCESS == event_result)
                {
                    if (NULL != eventdata)
                    {
                        characteristic = (GATT_CHARACTERISTIC_PARAM *)eventdata;

                        ga_event_data = (void *)&ga_serv_comp;
                        ga_event_datalen = sizeof(ga_serv_comp);

                        ga_char_info.uuid = characteristic->uuid.uuid_16;
                        ga_char_info.handle = characteristic->value_handle;
                        ga_char_info.property = characteristic->cproperty;

                        ga_serv_comp.service = \
                            (GA_BRR_SVC_INFO *)ga_brr_act_ctx[id].meta_info;
                        ga_serv_comp.characteristic = &ga_char_info;
                        ga_serv_comp.characteristic_count = 1;

                        ga_event_result = GA_CONTINUE;
                    }
                }

                if (GA_CONTINUE != ga_event_result)
                {
                    ga_event_result = event_result;

                    /* Free the allocated Service Info Meta Info */
                    BT_free_mem(ga_brr_act_ctx[id].meta_info);

                    /* Mark for activity Deletion */
                    del_act         = BT_TRUE;
                }
            }
        }
        break;

    case GATT_CHAR_DESC_DISCOVERY_RSP:

        /* Event */
        ga_event = GA_BRR_GET_CHARACTERISTIC_COMPOSITION_CNF;

        if (GA_BRR_MAX_ACTIVITY_CNTX != id)
        {
            /* Check if the Activity context Instance is waiting for this
             * particular event.
             */
            if (GATT_CHAR_DESC_DISCOVERY_RSP == ga_brr_act_ctx[id].expt_event)
            {
                /* Event Data */
                if (GA_SUCCESS == event_result)
                {
                    if (NULL != eventdata)
                    {
                        desc = (GATT_CHAR_DESC_PARAM *)eventdata;

                        ga_event_data = (void *)&ga_char_comp;
                        ga_event_datalen = sizeof(ga_char_comp);

                        ga_desc_info.uuid = desc->uuid.uuid_16;
                        ga_desc_info.handle = desc->handle;

                        ga_char_comp.desc = &ga_desc_info;
                        ga_char_comp.desc_count = 1;

                        ga_event_result = GA_CONTINUE;
                    }
                }

                if (GA_CONTINUE != ga_event_result)
                {
                    ga_event_result = event_result;

                    /* Free the allocated Service Info Meta Info */
                    BT_free_mem(ga_brr_act_ctx[id].meta_info);

                    /* Mark for activity Deletion */
                    del_act         = BT_TRUE;
                }
            }
        }
        break;

    case ATT_READ_RSP:

        /* Event */
        ga_event         = GA_BRR_READ_CHARACTERISTIC_CNF;
        ga_event_data    = eventdata;
        ga_event_datalen = event_datalen;

        /* Mark for activity Deletion */
        del_act         = BT_TRUE;

        break;

    case ATT_WRITE_RSP:
        /* Event */
        /* Assign the event based on the context */
        ga_event         = (BT_FALSE == ga_brr_act_ctx[id].process) ?
                           GA_BRR_WRITE_CHARACTERISTIC_CNF :
                           GA_BRR_CONFIG_CHARACTERISTIC_CNF;
        ga_event_data    = eventdata;
        ga_event_datalen = event_datalen;

        /* Mark for activity Deletion */
        del_act         = BT_TRUE;
        break;

    case ATT_WRITE_CMD_TX_COMPLETE:
        /* Mark for activity Deletion */
        del_act         = BT_TRUE;
        break;

    case ATT_ERROR_RSP:
        {
            UCHAR           opcode;
            ATT_ATTR_HANDLE t_hndl;
            UCHAR           err_code;
            UCHAR           * event_data;

            event_data = (UCHAR *) eventdata;

            if (NULL == event_data)
            {
                /* Should not reach here */
                break;
            }

            BT_UNPACK_LE_1_BYTE(&opcode  , event_data);
            BT_UNPACK_LE_2_BYTE(&t_hndl  , event_data+1);
            BT_UNPACK_LE_1_BYTE(&err_code, event_data+3);

            GA_BRR_PL_TRC (
            "[GA_BRR_PL] Received Error Response, for Op-Code 0x%02X for "
            "Handle 0x%04X, Rsp Code 0x%02X!\n", opcode, t_hndl,err_code);

            if ((ATT_WRITE_RSP == ga_brr_act_ctx[id].expt_event) ||
                (ATT_READ_RSP == ga_brr_act_ctx[id].expt_event))
            {
                if (ATT_WRITE_REQ == opcode)
                {
                    GA_BRR_PL_INF (
                    "[GA_BRR_PL] Received Error Response, for Write Req!\n");

                    /* Assign the event based on the context */
                    ga_event = (BT_FALSE == ga_brr_act_ctx[id].process) ?
                               GA_BRR_WRITE_CHARACTERISTIC_CNF :
                               GA_BRR_CONFIG_CHARACTERISTIC_CNF;
                }
                else if (ATT_READ_REQ == opcode)
                {
                    GA_BRR_PL_INF (
                    "[GA_BRR_PL] Received Error Response, for Read Req!\n");

                    ga_event = GA_BRR_READ_CHARACTERISTIC_CNF;
                }
                else
                {
                    GA_BRR_PL_ERR (
                    "[GA_BRR_PL] Received Error Response, for Unhandled"
                    "Op-Code 0x%02X for Handle 0x%04X, Rsp Code 0x%02X!\n",
                    opcode, t_hndl,err_code);
                }

                /* Assign the Error Response as Event Result */
                ga_event_result = err_code;

                /* Mark for activity Deletion */
                del_act         = BT_TRUE;
            }
        }
        break;

    case ATT_HANDLE_VALUE_NTF:
    case ATT_HANDLE_VALUE_IND:
        {
            UCHAR             * event_data;

            event_data = (UCHAR *) eventdata;

            if (NULL == event_data)
            {
                /* Should not reach here */
                break;
            }

            BT_UNPACK_LE_2_BYTE(&char_hndl, event_data);

            /* Event */
            ga_event         = GA_BRR_NOTIFICATION_IND;
            /** Adjust for 2 Bytes of Handle */
            ga_event_data    = (void *)(event_data + 2);
            ga_event_datalen = (event_datalen - 2);

            /* Pass Attribute Handle as Upper layer Context */
            ul_context = &char_hndl;

            /**
             * Marking the Instance ID as Invalid.
             * For Server Initiated ATT Transactions such as Notifications
             * and Indications, the Bearer Platform has not enough information
             * to map and identify the Profile ID of the module for which
             * this Notification/Indication is meaningly.
             *
             * The bearer internal should deliver this packet to all
             * registered profile.
             */
            inst_id          = GA_BRR_PROFILE_ID_INVALID;
        }
        break;

    default:
        /* Should not reach here */
        GA_BRR_PL_ERR(
        "[GA_BRR_PL]: Unknown ATT Event :0x%02x\n", att_event);
        break;
    }

    if (BT_TRUE == del_act)
    {
        /* Free the Activity Context */
        ga_brr_delete_activity_pl(ga_brr_act_ctx[id].pid);
    }

    ga_dev = ga_brr_dev_list_pl[pl_id].brr_dev;

    /* Unlock */
    BRR_UNLOCK();

    if (NULL != ga_brr_pl_cb)
    {
        /* Notify Upper Layer */
        ga_brr_pl_cb
        (
            &ga_dev,
            ul_context,
            role,
            (void *)&inst_id,
            ga_event,
            ga_event_result,
            ga_event_data,
            ga_event_datalen
        );
    }

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: << ga_gatt_cb_pl, retval 0x%04X\n", retval);

    return retval;
}

/* ----------------------------------------------------------------------- */
/* ======================Service Role Callbacks=========================== */
/* ----------------------------------------------------------------------- */
API_RESULT gatt_db_ga_gatt_char_handler
           (
               GATT_DB_HANDLE * handle,
               GATT_DB_PARAMS * params
           )
{
    API_RESULT         retval;
    GA_RESULT          ret;
    UINT16             config;
    UINT16             uuid;
    UCHAR              invoke_cb;
    BT_DEVICE_ADDR     bd_addr;
    GA_BRR_DEVICE      ga_dev;
    UCHAR              ga_event;
    UINT16             ga_event_result;
    void               * ga_event_data;
    UINT16             ga_event_datalen;
    UCHAR              pl_id;
    GA_BRR_CHR_CONTEXT char_cntx;

    retval           = GATT_DB_DONOT_RESPOND;
    ret              = GA_SUCCESS;
    invoke_cb        = BT_TRUE;
    ga_event_data    = NULL;
    ga_event_datalen = 0;
    ga_event_result  = GA_SUCCESS;
    ga_event         = GA_BRR_EVENT_INVALID;
    GA_mem_set(&char_cntx, 0x0, sizeof(GA_BRR_CHR_CONTEXT));

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: >> gatt_char_handler\n");

    BT_UNPACK_LE_2_BYTE(&uuid,GATT_DB_GET_UUID(params->handle));

    printf(
    "Received Requests from Peer on the Server Side with Op:0x%02X from "
    "DEVID:0x%02X Handle:0x%04X SID:0x%02X CID:0x%04X\n",
    params->db_op, handle->device_id, params->handle, handle->service_id,
    handle->char_id);

    /* Lock */
    BRR_LOCK();

    /**
     * 1. Find the BRR Device Associated with the device ID.
     * 2. Search the device list
     */
    /** Fetch BD Address */
    ret = ga_get_bd_addr_from_dev_hndl
          (
              &handle->device_id,
              &bd_addr
          );

    if(GA_SUCCESS != ret)
    {
        GA_BRR_PL_ERR (
        "[GA_BRR_PL]: Failed to Find BD Address for Device ID 0x%02X "
        "retval 0x%04X\n", handle->device_id, ret);
    }
    else
    {
        GA_BRR_PL_TRC(
        "[GA_BRR_PL] Peer Address :- " BT_DEVICE_ADDR_FRMT_SPECIFIER "\n",
        BT_DEVICE_ADDR_PRINT_STR(&bd_addr));
    }

    /**
     * TODO:
     * In future, this should be a search for a particular
     * ATT bearer instance.
     * Currently, the gatt_char_handler does not inform this layer
     * of the ATT Bearer intance on which a request is being made!
     */
    /* Fetch the GA BRR Device */
    ga_get_ga_dev_from_bd_addr
    (
        (void *)&bd_addr,
        NULL,
        &ga_dev
    );

    /* Search for the GA Bearer Device in the list */
    ret = ga_brr_search_dev_list_by_dev_pl
          (
              &ga_dev,
              &pl_id
          );

    if (GA_SUCCESS != ret)
    {
        GA_BRR_PL_ERR (
        "[GA_BRR_PL]: Search for BRR Device Instance in PL List Failed "
        "with retval 0x%04X\n", ret);

        /* Unlock */
        BRR_UNLOCK();

        /* Dropping the packet */
        return API_FAILURE;
    }

    switch (params->db_op)
    {
        case GATT_DB_CHAR_PEER_CLI_CNFG_WRITE_REQ:
            BT_UNPACK_LE_2_BYTE(&config, params->value.val);
            GA_BRR_PL_TRC(
            "[GA_BRR_PL]: CCCD set to 0x%04X for Handle: 0x%04X and UUID: "
            "0x%04X\n", config, params->handle, uuid);

            ga_event         = GA_BRR_CONFIG_CHARACTERISTIC_IND;
            ga_event_data    = &config;
            ga_event_result  = GA_SUCCESS;
            ga_event_datalen = sizeof(config);

            if (GATT_CLIENT_CONFIG == uuid)
            {
                /* Fetch Value from one previous handle */
                BT_UNPACK_LE_2_BYTE(&uuid,GATT_DB_GET_UUID((params->handle) - 1));
            }

            /**
             * Send the ATT Write Response for CCCD Write prior to invoking
             * callback to Upper Layers.
             * TODO:
             * - Validation of incoming event/write length and value of
             *   CCCD configuration being written needs to be done.
             * - Suitable ATT Error Response to be sent on failure.
             */
            ret = BT_att_write_rsp(&ga_brr_dev_list_pl[pl_id].att_handle)

            if(API_SUCCESS != ret)
            {
                GA_BRR_PL_ERR (
                "[GA_BRR_PL]: CCCD Write Response returned Error, retval 0x%04X\n",
                ret);
            }

            /* Assign the UL Context for this event */
            char_cntx.uuid   = uuid;
            char_cntx.inst   = handle->char_id;
            char_cntx.to_rsp = GA_FALSE;
            break;

        case GATT_DB_CHAR_PEER_WRITE_REQ:
            GA_BRR_PL_TRC(
            "[GA_BRR_PL]: Received Write for Handle: for Handle: 0x%04X and "
            " UUID: 0x%04X\n", params->handle, uuid);

            ga_event = GA_BRR_WRITE_CHARACTERISTIC_IND;
            ga_event_data    = params->value.val;
            ga_event_result  = GA_SUCCESS;
            ga_event_datalen = params->value.len;

            /* Assign the UL Context for this event */
            char_cntx.uuid   = uuid;
            char_cntx.inst   = handle->char_id;
            char_cntx.to_rsp = GA_TRUE;
            break;

        case GATT_DB_CHAR_PEER_WRITE_CMD:
            GA_BRR_PL_TRC(
            "[GA_BRR_PL]: Received Write WithOut Response for Handle: for "
            " Handle: 0x%04X, UUID: 0x%04X\n", params->handle, uuid);

            ga_event = GA_BRR_WRITE_CHARACTERISTIC_IND;
            ga_event_data    = params->value.val;
            ga_event_result  = GA_SUCCESS;
            ga_event_datalen = params->value.len;

            /* Assign the UL Context for this event */
            char_cntx.uuid   = uuid;
            char_cntx.inst   = handle->char_id;
            char_cntx.to_rsp = GA_FALSE;
            break;

        case GATT_DB_CHAR_PEER_READ_REQ:
            GA_BRR_PL_TRC(
            "[GA_BRR_PL]: Received Read for Handle: for Handle: 0x%04X and "
            " UUID: 0x%04X\n", params->handle, uuid);
            ga_event = GA_BRR_READ_CHARACTERISTIC_IND;

            /* Assign the UL Context for this event */
            char_cntx.uuid   = uuid;
            char_cntx.inst   = handle->char_id;
            char_cntx.to_rsp = GA_TRUE;
            break;

        case GATT_DB_CHAR_PEER_READ_BLOB_REQ:
            ga_event = GA_BRR_READ_BLOB_CHARACTERISTIC_IND;
            ga_event_data = &params->offset;
            ga_event_result = GA_SUCCESS;
            ga_event_datalen = GA_BRR_ATT_READ_BLOB_VALUE_OFFSET_LEN;

            /* Assign the UL Context for this event */
            char_cntx.uuid = uuid;
            char_cntx.inst = handle->char_id;
            char_cntx.to_rsp = GA_TRUE;
            break;

        default:
            retval    = API_SUCCESS;
            invoke_cb = BT_FALSE;
            break;
    }

    ga_dev = ga_brr_dev_list_pl[pl_id].brr_dev;

    /* Unlock */
    BRR_UNLOCK();

    if ((NULL != ga_brr_pl_cb) && (BT_TRUE == invoke_cb))
    {
        GA_BRR_PL_INF (
        "[GA_BRR_PL]: Server Callback for Service ID: 0x%02X and Char UUID in"
        " 0x%04X Char ID 0x%02X\n BRR DEV " GA_BRR_DEVICE_FRMT_SPECIFIER
        " and to_rsp set to %s\n", handle->service_id, char_cntx.uuid,
        char_cntx.inst, GA_BRR_DEVICE_PRINT_STR(&ga_brr_dev_list_pl[pl_id].brr_dev),
        ((GA_TRUE == char_cntx.to_rsp) ? "TRUE" : "FALSE"));

        /* Notify Upper Layer */
        if (GA_FAILURE == ga_brr_pl_cb
                          (
                              &ga_dev,
                              &char_cntx,
                              GA_BRR_SERVICE_ROLE,
                              (void *)&handle->service_id,
                              ga_event,
                              ga_event_result,
                              ga_event_data,
                              ga_event_datalen
                          ))
        {
            /*
             * Indicates that there is no response expected from App,
             * send error response from here
             */
            retval = (GA_FALSE == char_cntx.to_rsp) ? GATT_DB_DONOT_RESPOND : GA_FAILURE;
        }
    }

    GA_BRR_PL_TRC (
    "[GA_BRR_PL]: << gatt_char_handler, retval 0x%04X\n", retval);

    return retval;
}

/* Dynamic GATT db Initialization */
#ifdef GATT_DB_DYNAMIC
GA_RESULT ga_brr_dyn_gatt_db_init_pl
          (
              void * db_blob
          )
{
    GA_RESULT         retval;
#ifdef GATT_DB_HAVE_DB_SIGNATURE
    GATT_DB_SIGNATURE * db_signature;
#endif /* GATT_DB_HAVE_DB_SIGNATURE*/

    /* Initialize */
    retval       = API_SUCCESS;

#ifdef GATT_DB_HAVE_DB_SIGNATURE
    db_signature = NULL;
#endif /* GATT_DB_HAVE_DB_SIGNATURE*/

    GA_BRR_PL_TRC(
    "[GA_BRR_PL]: >> ga_brr_dyn_gatt_db_init_pl\n");

    /* Set the signature for GATT DB */
#ifdef GATT_DB_HAVE_DB_SIGNATURE

    if (NULL != db_blob)
    {
        /* Extract the DB Signature from the Parameters */
        db_signature = (GATT_DB_SIGNATURE *)db_blob;
    }

    if (NULL != db_signature)
    {
        retval = BT_gatt_db_dyn_set_db_signature
                 (
                    *db_signature
                 );

        GA_BRR_PL_TRC(
        "[GA_BRR_PL]: Set GATT DB Signature Status: 0x%04X\n", retval);
    }
    else
    {
        GA_BRR_PL_TRC(
        "[GA_BRR_PL]: ga_brr_dyn_gatt_db_init_pl() with NULL signature\n");
        return GATT_DB_NULL_PARAMETER_NOT_ALLOWED;
    }
#endif /* GATT_DB_HAVE_DB_SIGNATURE*/

    /* Register dynamic GATT DB */
    retval = BT_gatt_db_dyn_register();

    GA_BRR_PL_TRC(
    "[GA_BRR_PL]: GATT Database Registration Status: 0x%04X\n", retval);

    /* Register GA GATT DB Handler with PL Extension */
    (BT_IGNORE_RETURN_VALUE)gatt_db_init_pl(gatt_db_ga_gatt_char_handler);

    GA_BRR_PL_TRC(
    "[GA_BRR_PL]: << ga_brr_dyn_gatt_db_init_pl\n");

    return retval;
}
#endif /* GATT_DB_DYNAMIC */

#endif /* BT_GAM */
