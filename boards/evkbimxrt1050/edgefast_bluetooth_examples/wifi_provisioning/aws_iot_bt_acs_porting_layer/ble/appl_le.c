
/**
 *  \file appl_le.c
 *
 *  LE Application Source File
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "appl_le.h"
#include "appl_utils.h"

#include "appl_gatt_client.h"
#include "appl_l2cap_le.h"
#include "appl_smp.h"

#include "appl_service.h"

/* Configure logs for the functions in this file. */
#ifdef IOT_LOG_LEVEL_GLOBAL
    #define LIBRARY_LOG_LEVEL    IOT_LOG_LEVEL_GLOBAL
#else
    #define LIBRARY_LOG_LEVEL    IOT_LOG_NONE
#endif

#define LIBRARY_LOG_NAME         ( "APPL_LE" )
#include "iot_logging_setup.h"

#ifdef BT_LE

#ifdef APPL_FSM_SUPPORT
#include "appl_fsm.h"
#endif /* APPL_FSM_SUPPORT */

/* ----------------------------------------- External Global Variables */

/* ----------------------------------------- Exported Global Variables */
APPL_PEER_INSTANCE appl_peer_device_list[BT_MAX_REMOTE_DEVICES];

BT_DEVICE_ADDR peer_bd_addr =
               {
                   {0x40, 0xE3, 0x08, 0xDC, 0x1B, 0x00},

                   0x00
               };

/* ----------------------------------------- Static Global Variables */

#ifdef APPL_FSM_SUPPORT
/* APPL FSM entitiy */
DECL_STATIC DECL_CONST FSM_MODULE_TABLE_T appl_fsm =
{
    appl_access_state_handler,

    NULL,

    appl_event_handler_table,

    appl_state_event_table,

    APPL_MAX_NUM_EVENTS
};
#endif /* APPL_FSM_SUPPORT */

#ifdef APPL_FSM_SUPPORT
/** APPL FSM Identifier index */
static UCHAR appl_fsm_id;
#endif /* APPL_FSM_SUPPORT */

/* ------------------------------- Functions */
API_RESULT appl_fsm_post_event (EVENT_T fsm_event, void * param)
{
#ifdef APPL_FSM_SUPPORT
    return fsm_post_event (appl_fsm_id, fsm_event, param);
#else
    BT_IGNORE_UNUSED_PARAM(fsm_event);
    BT_IGNORE_UNUSED_PARAM(param);

    return API_SUCCESS;
#endif /* APPL_FSM_SUPPORT */
}

int appl_init(void)
{
#ifdef BT_LE
    UINT32     index;

#ifdef APPL_FSM_SUPPORT
    API_RESULT retval = API_SUCCESS;
    SET_ALL_APPL_STATE (SL_0_IDLE);
#endif /* APPL_FSM_SUPPORT */

    index = BT_MAX_REMOTE_DEVICES;

    do
    {
        index--;
        APPL_DEVICE_INIT(index);
    } while (index > 0);

#ifdef APPL_FSM_SUPPORT
    /* Register with stack FSM */
    retval = fsm_register_module (&appl_fsm, &appl_fsm_id);
    if (API_SUCCESS != retval)
    {
        IotLogError("[APPL]: FSM Registration Failed!\n");

        return -1;
    }

    /** Post Power on request to the application fsm */
    retval = appl_fsm_post_event
             (
                 ev_appl_device_init_req,
                 NULL
             );

    IotLogDebug (
    "[APPL]: FSM Post Event Result for Device Initialization Request = 0x%04X",
    retval);
#endif /* APPL_FSM_SUPPORT */
#endif /* BT_LE */

    return 0;
}

void appl_init_complete(void)
{
#ifdef GATT_DB
    void * config_ptr;
    DEVICE_HANDLE dhandle;
#ifdef BT_HAVE_GATT_DB_DYNAMIC_GLOBAL_ARRAY
    /**
     * Instance to inform GATT DB of the memory requirement.
     */
    GATT_DB_DYNAMIC_CONFIG  appl_bt_gatt_db_config;

    /* Initialize dynamic configuration structure with default values */
    GATT_DB_INIT_DYNAMIC_CONFIG(appl_bt_gatt_db_config);

    appl_bt_gatt_db_config.config_GATT_DB_DYN_UUID_ARRAY_SIZE = 512u;
    config_ptr = (void *)&appl_bt_gatt_db_config;
#else /* BT_HAVE_GATT_DB_DYNAMIC_GLOBAL_ARRAY */
    config_ptr = NULL;
#endif /* BT_HAVE_GATT_DB_DYNAMIC_GLOBAL_ARRAY */
#endif /* GATT_DB */

#ifdef BT_LE
    /* Register application with L2CAP */
    appl_l2cap_le_register ();

#ifdef SMP
    /* Register application security handling with SMP and configure */
    appl_smp_register ();
    appl_smp_configure_platform ();
#endif /* SMP */

#ifdef ATT
    /** Register with GATT */
    appl_gatt_init ();

#ifdef GATT_DB
    BT_gatt_db_init(config_ptr);

    dhandle = 0;
    appl_update_battery_lvl(&dhandle);
#endif /* GATT_DB */
#endif /* ATT */

    /* Initialize the Service */
    appl_service_init();

#endif /* BT_LE */
}

void appl_set_all_state(APPL_STATE_T state)
{
    UINT32 index;

    index = BT_MAX_REMOTE_DEVICES;
    do
    {
        index--;
        appl_set_state((APPL_HANDLE)index,state);
    } while (index > 0);
}

void appl_set_state(APPL_HANDLE  handle, APPL_STATE_T state)
{
    IotLogDebug (
    "Changing Application State. Previous 0x%02X, New 0x%02X\n",
    GET_APPL_STATE(handle), state);

    GET_APPL_STATE(handle) = state;
}

/** Application State Handler */
API_RESULT appl_access_state_handler
           (
                void       * param,
                STATE_T    * state
           )
{
    if ((NULL == param) ||
       (BT_MAX_REMOTE_DEVICES == ((APPL_EVENT_PARAM *)param)->handle))
    {
        /**
         *  For Device intitialization, power on and power off, there will not
         *  be any valid application instance handle, therefore use the first
         *  one as all application instances will have the same states during
         *  these stages
         */
        *state =  GET_APPL_STATE(0);
    }
    else
    {
        *state = GET_APPL_STATE(((APPL_EVENT_PARAM *)param)->handle);
    }

    return API_SUCCESS;
}


API_RESULT appl_add_device
           (
                /* IN */  ATT_HANDLE             * att_handle,
                /* OUT */ APPL_HANDLE            * handle
           )
{
    int index;
    API_RESULT   retval;

    index = BT_MAX_REMOTE_DEVICES;
    retval = API_FAILURE;

    do
    {
        index--;
        /* Allocates assuming duplication does not occur */
        if (DEVICE_HANDLE_INIT_VAL == APPL_GET_DEVICE_HANDLE(index))
        {
             APPL_GET_ATT_INSTANCE(index) = (*att_handle);
             (*handle) = (APPL_HANDLE) index;
             retval = API_SUCCESS;
             break;
        }
        else
        {
            /* Check if its a ATT level Reconnection on an exisiting ACL Link */
            if(APPL_GET_DEVICE_HANDLE(index) == (att_handle->device_id))
            {
                APPL_GET_ATT_INSTANCE(index) = (*att_handle);
                ( *handle ) = (APPL_HANDLE) index;
                retval = API_SUCCESS;
                break;
            }
        }
    }while(index > 0);

    return retval;
}

API_RESULT appl_set_cxn_handle
           (
               /* IN */ UINT16           cxn_handle,
               /* IN */ DEVICE_HANDLE    dq_handle,
               /* OUT */ APPL_HANDLE     * handle
           )

{
    API_RESULT  retval;

    retval = appl_get_handle_from_device_handle (dq_handle,handle);

    if (API_SUCCESS == retval)
    {
        APPL_GET_CONNECTION_HANDLE((*handle)) = cxn_handle;
    }

    return retval;
}


API_RESULT appl_get_handle_from_cxn_handle
           (
                /* IN */  UINT16         cxn_handle,
                /* OUT */ APPL_HANDLE    * handle
           )
{
    int          index;
    API_RESULT   retval;

    index = BT_MAX_REMOTE_DEVICES;
    retval = API_FAILURE;

    do
    {
        index--;
        if (cxn_handle == APPL_GET_CONNECTION_HANDLE(index))
        {
            ( *handle ) = (APPL_HANDLE)( index );
            retval = API_SUCCESS;
            break;
        }
    }while(index > 0);

    return retval;
}


API_RESULT appl_get_handle_from_device_handle
           (
                /* IN */  DEVICE_HANDLE    dq_handle,
                /* OUT */ APPL_HANDLE      * handle
           )
{
    int          index;
    API_RESULT   retval;

    index = BT_MAX_REMOTE_DEVICES;
    retval = API_FAILURE;

    do
    {
        index--;
        if (dq_handle == APPL_GET_DEVICE_HANDLE(index))
        {
            (*handle) = (APPL_HANDLE)(index);
            retval = API_SUCCESS;
            break;
        }
    }while(index > 0);

    return retval;
}


API_RESULT appl_get_cxn_handle_from_device_handle
           (
                /* IN */  DEVICE_HANDLE    dq_handle,
                /* OUT */ UINT16      * handle
           )
{
    int          index;
    API_RESULT   retval;

    index = BT_MAX_REMOTE_DEVICES;
    retval = API_FAILURE;

    do
    {
        index--;
        if (dq_handle == APPL_GET_DEVICE_HANDLE(index))
        {
            (*handle) = (UINT16)(APPL_GET_CONNECTION_HANDLE(index));
            retval = API_SUCCESS;
            break;
        }
    }while(index > 0);

    return retval;
}

API_RESULT appl_delete_device
           (
                /* IN */ UINT16 cxn_handle
           )
{
    int index; API_RESULT   retval;

    index = BT_MAX_REMOTE_DEVICES;
    retval = API_FAILURE;

    do
    {
        index--;
        if (cxn_handle == APPL_GET_CONNECTION_HANDLE(index))
        {
            APPL_DEVICE_INIT(index);
            retval = API_SUCCESS;
            break;
        }
    }while(index > 0);

    return retval;
}


API_RESULT appl_set_att_instance
           (
               /* IN */  ATT_HANDLE     * att_instance,
               /* OUT */ APPL_HANDLE    * handle
           )
{
    API_RESULT retval;

    retval = appl_get_handle_from_device_handle (att_instance->device_id,handle);

    if (API_SUCCESS == retval)
    {
        APPL_SET_ATT_INSTANCE ((*handle),(*att_instance));
    }

    return retval;
}

void appl_display_all_connected_devices(void)
{
    UINT32 i;
    UCHAR  count = 0;

    IotLogDebug("\n\n***************************************\n");
    IotLogDebug("List of Current Connected devices:\n");
    IotLogDebug("***************************************\n\n");

    for (i = (BT_MAX_REMOTE_DEVICES - 1); (i > 0); i--)
    {
        if (HCI_INVALID_CONNECTION_HANDLE != APPL_GET_CONNECTION_HANDLE(i))
        {
            IotLogDebug("%d. ApplHndl:%d, CxnHndl:0x%04X, DevHndl:0x%2X\n",
            (BT_MAX_REMOTE_DEVICES - i), i, APPL_GET_CONNECTION_HANDLE(i),
            APPL_GET_DEVICE_HANDLE(i));

            count += 1;
        }
    }

    if (0 == count)
    {
        IotLogDebug("No Devices Connected. Currently, List EMPTY!\n");
    }
}


#ifdef BT_DUAL_MODE
/*
 * Update Service Handle Range in SDP record
 * and activate SDP record.
 */
API_RESULT appl_update_and_activate_sdp_record
            (
                /* IN */ UCHAR service_type,
                /* IN */ ATT_ATTR_HANDLE start_handle,
                /* IN */ ATT_ATTR_HANDLE end_handle
            )
{
    UINT32 record_handle;
    API_RESULT retval;

    /* local variable to extract the supported features */
    UCHAR protocol_desc_list[] = {0x35, 0x13, 0x35, 0x06, 0x19,
                                  /* UUID L2CAP */
                                  0x01, 0x00,
                                  0x09,
                                  /* PSM = ATT */
                                  0x00, 0x1F,
                                  0x35, 0x09, 0x19,
                                  /* UUID ATT */
                                  0x00, 0x07,
                                  0x09,
                                  /* Start Handle */
                                  0x00, 0x1E,
                                  0x09,
                                  /* End Handle */
                                  0x00, 0x28};

    /* Get Record Handle from SDP Database */
    retval = BT_dbase_get_record_handle (service_type, 0, &record_handle);

    if (API_SUCCESS == retval)
    {
        /* Update Start Handle */
        BT_PACK_BE_2_BYTE_VAL(&protocol_desc_list[16], start_handle);

        /* Update End Handle */
        BT_PACK_BE_2_BYTE_VAL(&protocol_desc_list[19], end_handle);

        retval = BT_dbase_update_attr_value
                 (
                     record_handle,
                     0x0004,
                     protocol_desc_list,
                     sizeof (protocol_desc_list)
                 );

        if (API_SUCCESS == retval)
        {
            retval = BT_dbase_activate_record(record_handle);
        }
    }
    return retval;
}

#if 0
API_RESULT appl_set_gatt_service_in_sdp_record
           (
               UCHAR service_instance,
               UCHAR sdp_instance
           )
{
    GATT_DB_SERVICE service_info;
    API_RESULT retval;

    /* Get the GATT Service Info of the service */
    retval = appl_gatt_db_find_service_info_from_inst
             (
                service_instance,
                &service_info
             );

    if (API_SUCCESS == retval)
    {
         /**
          * Update Service Handle Range in SDP record
          * and activate SDP record
          */
        retval = appl_update_gatt_service_range_in_sdp_record
                 (
                    sdp_instance,
                    service_info.start_handle,
                    service_info.end_handle
                 );
    }
    else
    {
        IotLogDebug("[** ERR **]: Failed to find the service info, reason 0x%04X\n",
        retval);
    }

    return retval;
}
#endif
#endif /* BT_DUAL_MODE */
#endif /* BT_LE */

