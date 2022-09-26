
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
#include "appl_sdp.h"

#ifdef BT_LE

#ifdef APPL_FSM_SUPPORT
#include "appl_fsm.h"
#endif /* APPL_FSM_SUPPORT */

/* ----------------------------------------- External Global Variables */

/* ----------------------------------------- Exported Global Variables */
APPL_PEER_INSTANCE appl_peer_device_list[ATT_MAX_CONNECTION_INSTANCES];

BT_DEVICE_ADDR peer_bd_addr =
               {
                   {0x40U, 0xE3U, 0x08U, 0xDCU, 0x1BU, 0x00U},

                   0x00U
               };

/* ----------------------------------------- Static Global Variables */

/* Global to hold the current GAP role Selection */
DECL_STATIC UCHAR appl_le_gap_central_role;

#ifdef APPL_FSM_SUPPORT
/* APPL FSM entity */
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

void appl_update_config_peer_addr(BT_DEVICE_ADDR * updt_addr)
{
    if (NULL != updt_addr)
    {
        /* Copy the Updated Address into the Global Config */
        BT_COPY_BD_ADDR_AND_TYPE(&peer_bd_addr, updt_addr);
    }

    CONSOLE_OUT(
    "\nDefault Config Peer Address Updated to :- " BT_DEVICE_ADDR_FRMT_SPECIFIER "\n",
    BT_DEVICE_ADDR_PRINT_STR(&peer_bd_addr));
}

void appl_set_gap_central_role(UCHAR flag)
{
    /**
     * Set the GAP Central Role flag to
     * 1. BT_TRUE : Central Role Enabled. The Application on
     *              - On receiving an ADV Report check of the
     *                desired "peer_bd_addr" and Auto Connects.
     *              - On Disconnection, starts Scanning automatically.
     * 2. BT_FALSE : Central Role Disabled. Peripheral Role Set.
     *               The Application starts to Advertise and on
     *               disconnection starts to Advertise automatically.
     */
    appl_le_gap_central_role = (BT_TRUE == flag) ? BT_TRUE : BT_FALSE;

    CONSOLE_OUT("\nDefault GAP Role Set to %s\n",
    (BT_TRUE == appl_le_gap_central_role) ? "CENTRAL" : "PERIPHERAL");
}

UCHAR appl_is_gap_central_role(void)
{
    return appl_le_gap_central_role;
}

int appl_init(void)
{
#ifdef BT_LE
    UINT32     index;

#ifdef APPL_FSM_SUPPORT
    API_RESULT retval = API_SUCCESS;
    SET_ALL_APPL_STATE (SL_0_IDLE);
#endif /* APPL_FSM_SUPPORT */

    /** Set Default GAP Role as Peripheral */
    APPL_SET_GAP_PERIPHERAL_ROLE();

    index = ATT_MAX_CONNECTION_INSTANCES;

    do
    {
        index--;
        APPL_DEVICE_INIT(index);
    } while (index > 0U);

#ifdef APPL_FSM_SUPPORT
    /* Register with stack FSM */
    retval = fsm_register_module (&appl_fsm, &appl_fsm_id);
    if (API_SUCCESS != retval)
    {
        APPL_ERR("[APPL]: FSM Registration Failed!\n");

        return -1;
    }

    /** Post Power on request to the application fsm */
    retval = appl_fsm_post_event
             (
                 ev_appl_device_init_req,
                 NULL
             );

    APPL_TRC (
    "[APPL]: FSM Post Event Result for Device Initialization Request = 0x%04X",
    retval);
#endif /* APPL_FSM_SUPPORT */

    /* Initialize GATT DB Module */
    appl_gatt_db_module_init();

    /* Initialize the Service */
    appl_service_init();

#endif /* BT_LE */

    return 0U;
}

void appl_init_complete(void)
{
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
#endif /* ATT */

    /* Inform BT ON Complete to Service layer */
    appl_service_bt_init();

#endif /* BT_LE */
}

void appl_gatt_db_module_init(void)
{
#ifdef GATT_DB
    void * config_ptr;

#ifdef BT_HAVE_GATT_DB_DYNAMIC_GLOBAL_ARRAY
    /**
     * Instance to inform GATT DB of the memory requirement.
     */
    GATT_DB_DYNAMIC_CONFIG  appl_bt_gatt_db_config;

    /* Initialize dynamic configuration structure with default values */
    GATT_DB_INIT_DYNAMIC_CONFIG(appl_bt_gatt_db_config);

    config_ptr = (void *)&appl_bt_gatt_db_config;
#else /* BT_HAVE_GATT_DB_DYNAMIC_GLOBAL_ARRAY */
    config_ptr = NULL;
#endif /* BT_HAVE_GATT_DB_DYNAMIC_GLOBAL_ARRAY */

    /* Initialize the GATT DB Module */
    BT_gatt_db_init(config_ptr);

#endif /* GATT_DB */
}

void appl_gatt_db_module_deinit(void)
{
#ifdef GATT_DB
    /** De-initialize the GATT DB Module */
    BT_gatt_db_deinit();
#endif /* GATT_DB */
}

void appl_niface_deinit(void)
{
    /*de-initialize network interface module*/
    niface_setup_down();
    niface_stack_down();
}

void appl_deinit(void)
{
    /*de-initialize network interface module*/
    appl_niface_deinit();

    /* De-initialize GATT DB Module */
    appl_gatt_db_module_deinit();

    /* Inform BT OFF to Service layer */
    appl_service_bt_deinit();
}

void appl_set_all_state(APPL_STATE_T state)
{
    UINT32 index;

    index = ATT_MAX_CONNECTION_INSTANCES;
    do
    {
        index--;
        appl_set_state((APPL_HANDLE)index,state);
    } while (index > 0U);
}

void appl_set_state(APPL_HANDLE  handle, APPL_STATE_T state)
{
    APPL_TRC (
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
       (ATT_MAX_CONNECTION_INSTANCES == ((APPL_EVENT_PARAM *)param)->handle))
    {
        /**
         *  For Device initialization, power on and power off, there will not
         *  be any valid application instance handle, therefore use the first
         *  one as all application instances will have the same states during
         *  these stages
         */
        *state =  GET_APPL_STATE(0U);
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

    index = ATT_MAX_CONNECTION_INSTANCES;
    retval = API_FAILURE;

    do
    {
        index--;
        /* Allocates assuming duplication does not occur */
        if (DEVICE_HANDLE_INIT_VAL == APPL_GET_DEVICE_HANDLE(index))
        {
             APPL_GET_ATT_INSTANCE(index) = (*att_handle);

             APPL_SET_MTU(index, ATT_DEFAULT_MTU);

             (*handle) = (APPL_HANDLE) index;
             retval = API_SUCCESS;
             break;
        }
        else
        {
            /* Check if its a ATT level Reconnection on an existing ACL Link */
            if(APPL_GET_DEVICE_HANDLE(index) == (att_handle->device_id))
            {
                APPL_GET_ATT_INSTANCE(index) = (*att_handle);
                ( *handle ) = (APPL_HANDLE) index;
                retval = API_SUCCESS;
                break;
            }
        }
    }while(index > 0U);

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

    index = ATT_MAX_CONNECTION_INSTANCES;
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
    }while(index > 0U);

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

    index = ATT_MAX_CONNECTION_INSTANCES;
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
    }while(index > 0U);

    return retval;
}

API_RESULT appl_delete_device
           (
                /* IN */ UINT16 cxn_handle
           )
{
    int index; API_RESULT   retval;

    index = ATT_MAX_CONNECTION_INSTANCES;
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
    }while(index > 0U);

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
    int i;
    UCHAR  count;
    BT_DEVICE_ADDR t_bd_addr;

    /* Initialize */
    count = 0U;
    i     = ATT_MAX_CONNECTION_INSTANCES;
    BT_mem_set(&t_bd_addr, 0x0, sizeof(t_bd_addr));

    APPL_TRC("\n\n***************************************\n");
    APPL_TRC("List of Current Connected devices:\n");
    APPL_TRC("***************************************\n\n");

    do
    {
        i--;

        if (HCI_INVALID_CONNECTION_HANDLE != APPL_GET_CONNECTION_HANDLE(i))
        {
            /* Get the peer BD Address for the device handle received */
            (BT_IGNORE_RETURN_VALUE) device_queue_get_remote_addr (&APPL_GET_DEVICE_HANDLE(i), &t_bd_addr);

            APPL_TRC("%d. Peer with - \n", (ATT_MAX_CONNECTION_INSTANCES - i));
            APPL_TRC("\t ->         APPL_HANDLE: %d\n", i);
            APPL_TRC("\t ->   Connection Handle: 0x%04X\n", APPL_GET_CONNECTION_HANDLE(i));
            APPL_TRC("\t ->                " BT_DEVICE_ADDR_SPACED_FRMT_SPECIFIER "\n",
            BT_DEVICE_ADDR_PRINT_STR(&t_bd_addr));
            APPL_TRC("\t ->         ATT Handle : DEV_ID - 0x%02X ATT_ID - 0x%02X\n",
            APPL_GET_ATT_INSTANCE(i).device_id, APPL_GET_ATT_INSTANCE(i).att_id);

            count += 1U;
        }

    }while(i > 0U);

    if (0U == count)
    {
        APPL_TRC("No Devices Connected. Currently, List EMPTY!\n");
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
    UCHAR protocol_desc_list[] = {0x35U, 0x13U, 0x35U, 0x06U, 0x19U,
                                  /* UUID L2CAP */
                                  0x01U, 0x00U,
                                  0x09U,
                                  /* PSM = ATT */
                                  0x00U, 0x1FU,
                                  0x35U, 0x09U, 0x19U,
                                  /* UUID ATT */
                                  0x00U, 0x07U,
                                  0x09U,
                                  /* Start Handle */
                                  0x00U, 0x1EU,
                                  0x09U,
                                  /* End Handle */
                                  0x00U, 0x28U};

    /* Get Record Handle from SDP Database */
    retval = BT_dbase_get_record_handle (service_type, 0U, &record_handle);

    if (API_SUCCESS == retval)
    {
        /* Update Start Handle */
        BT_PACK_BE_2_BYTE_VAL(&protocol_desc_list[16U], start_handle);

        /* Update End Handle */
        BT_PACK_BE_2_BYTE_VAL(&protocol_desc_list[19U], end_handle);

        retval = BT_dbase_update_attr_value
                 (
                     record_handle,
                     0x0004U,
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
        APPL_TRC("[** ERR **]: Failed to find the service info, reason 0x%04X\n",
        retval);
    }

    return retval;
}

#endif /* BT_DUAL_MODE */
#endif /* BT_LE */

